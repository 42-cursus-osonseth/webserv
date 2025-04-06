window.onload = function() {
    var couleur = document.cookie.replace(/(?:(?:^|.*;\s*)couleur\s*\=\s*([^;]*).*$)|^.*$/, "$1");
    var colorMap = {
        "Vert": "green",
        "Bleu": "blue",
        "Violet": "purple"
    };

    var header = document.querySelector("header");
    if (couleur && header) {
        var cssColor = colorMap[couleur];
        if (cssColor)
            header.style.backgroundColor = cssColor;
         else 
            header.style.backgroundColor = "black";
    } 
    else if (header) 
        header.style.backgroundColor = "black";
        
};
