// Función para el botón mágico
const magicBtn = document.getElementById('magic-btn');
if (magicBtn) {
    magicBtn.addEventListener('click', function () {
        const colores = ['#bb86fc', '#03dac6', '#cf6679', '#ffb74d'];
        const colorRandom = colores[Math.floor(Math.random() * colores.length)];

        // Cambia el color del título al azar
        const title = document.getElementById('title');
        if (title) title.style.color = colorRandom;

        alert("¡Gracias por apoyar el talento de Buhodermia! 🦉✨");
    });
}

// Función para mostrar letras en modal
function mostrarLetra(titulo, letra) {
    const modal = document.getElementById('lyrics-modal');
    if (!modal) return;
    
    const tituloElemento = document.getElementById('modal-song-title');
    const textoElemento = document.getElementById('modal-song-lyrics');
    const reproductor = document.getElementById('audio-player');

    if (tituloElemento) tituloElemento.innerText = titulo;
    if (textoElemento) textoElemento.innerText = letra;
    
    modal.style.display = "block";
    
    // ESTO ACTIVA LA MÚSICA
    if (reproductor) reproductor.play(); 
}

// Función para cerrar letras
function cerrarLetra() {
    const modal = document.getElementById('lyrics-modal');
    const reproductor = document.getElementById('audio-player');
    const playBtn = document.getElementById('play-btn');

    if (modal) modal.style.display = "none";
    
    // ESTO PAUSA LA MÚSICA AL CERRAR
    if (reproductor) {
        reproductor.pause();
        reproductor.currentTime = 0;
    }
    
    if (playBtn) {
        playBtn.innerHTML = '<span class="icon">▶</span> ESCUCHAR AHORA';
    }
}

// Control de reproducción de audio
document.addEventListener('DOMContentLoaded', () => {
    const playBtn = document.getElementById('play-btn');
    const audio = document.getElementById('audio-player');
    
    if (playBtn && audio) {
        const icon = playBtn.querySelector('.icon');

        playBtn.addEventListener('click', () => {
            if (audio.paused) {
                audio.play();
                playBtn.style.background = "#2dd4bf";
                playBtn.style.borderColor = "#2dd4bf";
                if (icon) icon.innerText = '‖';
                playBtn.innerHTML = '<span class="icon">‖</span> PAUSAR';
            } else {
                audio.pause();
                playBtn.style.background = "rgba(168, 85, 247, 0.1)";
                playBtn.style.borderColor = "#a855f7";
                if (icon) icon.innerText = '▶';
                playBtn.innerHTML = '<span class="icon">▶</span> ESCUCHAR AHORA';
            }
        });
    }
});

// Cursor personalizado
document.addEventListener('mousemove', (e) => {
    const cursor = document.getElementById('cursor-custom');
    if (cursor) {
        cursor.style.left = e.clientX - 10 + 'px';
        cursor.style.top = e.clientY - 10 + 'px';
    }
});
});