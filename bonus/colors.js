
window.onload = function() {
    var couleur = document.cookie.replace(/(?:(?:^|.*;\s*)couleur\s*\=\s*([^;]*).*$)|^.*$/, "$1");
    
    if (couleur) {
        document.body.style.backgroundColor = couleur;
    }
};
