let timeout;

function resetTimeout() {
    clearTimeout(timeout);
    timeout = setTimeout(() => {
        window.location.href = "/disconnect.html"; 
    }, 10000); // 60 secondes
}

// Réinitialiser seulement si une action significative est détectée
function userInteracted() {
    resetTimeout();
}

// Réinitialiser sur les requêtes réseau (XHR, Fetch, Navigation)
document.addEventListener("click", (event) => {
    if (event.target.tagName === "A" || event.target.tagName === "BUTTON") {
        userInteracted();
    }
});

// Détecter les requêtes réseau
(function(open) {
    XMLHttpRequest.prototype.open = function() {
        userInteracted();
        open.apply(this, arguments);
    };
})(XMLHttpRequest);

(function(fetch) {
    window.fetch = function() {
        userInteracted();
        return fetch.apply(this, arguments);
    };
})(window.fetch);

// Initialisation du timeout
resetTimeout();

