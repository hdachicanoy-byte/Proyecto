#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// ==========================================
// DEFINICIÓN DE PINES
// ==========================================
const int pirPin = 25;   // Sensor Mini PIR SR505
const int reedPin = 4;   // Sensor Magnético KY-021 (Pin S)
const int relayPin = 5; // Módulo Relé (Controla las Luces LED 5V)
const int ledPin = 2;    // LED Indicador de la placa ESP32

// ==========================================
// CREDENCIALES DE RED Y FIREBASE
// ==========================================
#define WIFI_SSID "Aula1_Sis"
#define WIFI_PASSWORD "Tecnico2026"
#define API_KEY  "AIzaSyBzZaccsfkMWhXA9dcnQy4CEnQUPgalXvQ"
#define DATABASE_URL "https://apex20-bbee2-default-rtdb.firebaseio.com"

// Objetos de Firebase
FirebaseData fbdo;
FirebaseData streamDataObj; 
FirebaseAuth auth;
FirebaseConfig config;

// Variables de estado
int pirState = LOW;
int lastPirState = LOW;

bool relayManualState = false; // Estado manual activado desde la web
bool currentRelayState = false; // Estado actual físico del relé

// Variables para el sensor magnético (Debounce)
int reedState = HIGH;   
int lastReading = HIGH; 
unsigned long lastChange = 0;
const unsigned long debounceMs = 20;

// ==========================================
// FUNCIÓN PARA ACTUALIZAR EL RELÉ Y FIREBASE
// ==========================================
void updateRelayState(bool targetState) {
  if (currentRelayState != targetState) {
    currentRelayState = targetState;
    digitalWrite(relayPin, currentRelayState ? HIGH : LOW);
    Serial.printf("Relé (Luces 5V) actualizado a: %s\n", currentRelayState ? "ON" : "OFF");

    // Sincronizar estado real con Firebase para reflejarlo en la Web
    if (Firebase.RTDB.setBool(&fbdo, "/actuadores/rele", currentRelayState)) {
      Serial.println("Estado del relé sincronizado con Firebase.");
    }
  }
}

// ==========================================
// CALLBACKS DE STREAM DE FIREBASE
// ==========================================
void streamCallback(FirebaseStream data) {
  String ruta = data.dataPath();

  if (data.dataType() == "boolean") {
    bool estado = data.boolData();

    if (ruta == "/rele" || ruta == "/") {
      relayManualState = estado;
      Serial.printf("Comando manual Web para relé: %s\n", relayManualState ? "ON" : "OFF");

      // Evaluar estado combinando web y sensor PIR
      bool stateToApply = relayManualState || (pirState == HIGH);
      updateRelayState(stateToApply);
    }
    else if (ruta == "/led") {
      digitalWrite(ledPin, estado ? HIGH : LOW);
      Serial.printf("Estado del LED indicador actualizado a: %s\n", estado ? "ON" : "OFF");
    }
  }
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("El Stream de Firebase expiró por tiempo de espera. Reconectando...");
  }
}

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);

  // Configuración de Entradas y Salidas
  pinMode(pirPin, INPUT);
  pinMode(reedPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(relayPin, LOW);
  digitalWrite(ledPin, LOW);

  // Conexión WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando a Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n¡Conectado a la red WiFi!");
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());

  // Configuración de Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase SignUp OK");
  } else {
    Serial.printf("Firebase SignUp error: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Inicialización de Stream para escuchar comandos desde la WEB
  if (!Firebase.RTDB.beginStream(&streamDataObj, "/actuadores")) {
    Serial.printf("Error en Stream: %s\n", streamDataObj.errorReason().c_str());
  }

  Firebase.RTDB.setStreamCallback(&streamDataObj, streamCallback, streamTimeoutCallback);

  Serial.println("--- Sistema ZEUS APEX Inicializado ---");
}

// ==========================================
// LOOP PRINCIPAL
// ==========================================
void loop() {
  unsigned long currentMillis = millis();

  // 1. SENSOR DE PRESENCIA (PIR) Y CONTROL DE ILUMINACIÓN
  pirState = digitalRead(pirPin);

  if (pirState != lastPirState) {
    lastPirState = pirState;

    if (pirState == HIGH) {
      digitalWrite(ledPin, HIGH);
      Serial.println("PIR: Presencia detectada -> Encendiendo luces LED");
    } else {
      digitalWrite(ledPin, LOW);
      Serial.println("PIR: Sin presencia");
    }

    // Encender relé si hay movimiento O si se activó manualmente desde la Web
    bool shouldRelayBeOn = (pirState == HIGH) || relayManualState;
    updateRelayState(shouldRelayBeOn);

    // Notificar presencia a Firebase
    Firebase.RTDB.setInt(&fbdo, "/sensor/movimiento", pirState);

  }
  // 2. SENSOR MAGNÉTICO (PUERTA)
  int reading = digitalRead(reedPin);

  if (reading != lastReading) {
    lastChange = currentMillis;
    lastReading = reading;
  } 

  if ((currentMillis - lastChange) > debounceMs) {
    if (reading != reedState) {
      reedState = reading;

      if (reedState == HIGH) {
        Serial.println("ALERTA: Puerta ABIERTA (Imán retirado)");
      } else {
        Serial.println("Magnético: Puerta CERRADA (Imán detectado)");
      }

      // Notificar estado de la puerta a Firebase
      Firebase.RTDB.setInt(&fbdo, "/sensor/iman", reedState);
    }
  }

  delay(10);
}