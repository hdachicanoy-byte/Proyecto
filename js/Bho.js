document.getElementById('magic-btn').addEventListener('click', function () {
    const colores = ['#bb86fc', '#03dac6', '#cf6679', '#ffb74d'];
    const colorRandom = colores[Math.floor(Math.random() * colores.length)];

    // Cambia el color del título al azar
    document.getElementById('title').style.color = colorRandom;

    alert("¡Gracias por apoyar el talento de Buhodermia! 🦉✨");
});
function mostrarLetra(titulo, letra) {
    const modal = document.getElementById('lyrics-modal');
    const tituloElemento = document.getElementById('modal-song-title');
    const textoElemento = document.getElementById('modal-song-lyrics');
    const reproductor = document.getElementById('audio-player');

    tituloElemento.innerText = titulo;
    textoElemento.innerText = letra;
    
    modal.style.display = "block";
    
    // ESTO ACTIVA LA MÚSICA
    reproductor.play(); 
}

function cerrarLetra() {
    const modal = document.getElementById('lyrics-modal');
    const reproductor = document.getElementById('audio-player');

    modal.style.display = "none";
    
    // ESTO PAUSA LA MÚSICA AL CERRAR
    reproductor.pause();
    reproductor.currentTime = 0; // Reinicia la canción al principio
}
ocument.addEventListener('DOMContentLoaded', () => {
    const playBtn = document.getElementById('play-btn');
    const audio = document.getElementById('audio-player');
    const icon = playBtn.querySelector('.icon');

    playBtn.addEventListener('click', () => {
        if (audio.paused) {
            audio.play();
            playBtn.style.background = "#2dd4bf"; // Cambia a turquesa al sonar
            playBtn.style.borderColor = "#2dd4bf";
            playBtn.innerHTML = '<span class="icon">‖</span> PAUSAR';
        } else {
            audio.pause();
            playBtn.style.background = "rgba(168, 85, 247, 0.1)";
            playBtn.style.borderColor = "#a855f7";
            playBtn.innerHTML = '<span class="icon">▶</span> ESCUCHAR AHORA';
        }
    });

    // Resetear el botón cuando se cierra la ventana modal
    // (Asegúrate de llamar a esta lógica en tu función cerrarLetra)
});

// En tu función cerrarLetra() añade esto:
function cerrarLetra() {
    const audio = document.getElementById('audio-player');
    const playBtn = document.getElementById('play-btn');
    
    audio.pause();
    audio.currentTime = 0;
    playBtn.innerHTML = '<span class="icon">▶</span> ESCUCHAR AHORA';
    document.getElementById('lyrics-modal').style.display = 'none';
}
document.addEventListener('DOMContentLoaded', () => {
    const playBtn = document.getElementById('play-btn');
    const audio = document.getElementById('audio-player');
    const icon = document.getElementById('icon');
    const text = document.getElementById('text');

    playBtn.addEventListener('click', () => {
        if (audio.paused) {
            // SI ESTÁ PAUSADO: Dale Play
            audio.play();
            playBtn.classList.add('playing');
            icon.innerText = '‖'; // Símbolo de pausa
            text.innerText = 'PAUSAR';
        } else {
            // SI ESTÁ SONANDO: Dale Pausa
            audio.pause();
            playBtn.classList.remove('playing');
            icon.innerText = '▶'; // Símbolo de play
            text.innerText = 'REPRODUCIR';
        }
    });
});

document.addEventListener('mousemove', (e) => {
    const cursor = document.getElementById('cursor-custom');
    cursor.style.left = e.clientX - 10 + 'px';
    cursor.style.top = e.clientY - 10 + 'px';
});