#!/usr/bin/env php
<?php
while(1);
$query_string = getenv('QUERY_STRING');

if (empty($query_string)) {
    date_default_timezone_set("Europe/Paris");

    $date = date("Y-m-d");
    $time = date("H:i:s");

    $html_body = <<<HTML
<!DOCTYPE html>
<html>
<head>
    <meta charset='UTF-8'>
    <title>Affichage Date et Heure</title>
    <style>
        body {
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            text-align: center;
            font-family: Arial, sans-serif;
            background: linear-gradient(to right, #00c6ff, #007f00);
            color: white;
        }
        h1 {
            font-size: 2em;
        }
        p {
            font-size: 1.5em;
        }
    </style>
</head>
<body>
    <div>
        <h1>Date et Heure</h1>
        <p>$date - <span id='heure'>$time</span></p>
    </div>
    <script>
        function updateTime() {
            let now = new Date();
            let heureActuelle = now.toLocaleTimeString();
            document.getElementById('heure').innerText = heureActuelle;
        }
        setInterval(updateTime, 1000);
        updateTime();
    </script>
</body>
</html>
HTML;
    // while (1) { echo "cc"; }
    $length = strlen($html_body);

    echo "HTTP/1.1 200 OK\r\n";
    echo "Content-Type: text/html; charset=UTF-8\r\n";
    echo "Content-Length: $length\r\n\r\n";
    echo $html_body;
} else {
    parse_str($query_string, $params);

    if (isset($params['select'])) {
        $select = htmlspecialchars($params['select']);
        $responseHeader = "HTTP/1.1 302 Found\r\n";
        $responseHeader .= "Location: /colors.html\r\n";
        $responseHeader .= "Set-Cookie: couleur=" . $select . "; Path=/; Max-Age=31536000\r\n";
        $responseHeader .= "Content-Length: 0\r\n\r\n";
        echo $responseHeader;
    } else {
        $error = "<html><body><h1>Paramètre 'select' manquant</h1></body></html>";
        $length = strlen($error);
        echo "HTTP/1.1 400 Bad Request\r\n";
        echo "Content-Type: text/html\r\n";
        echo "Content-Length: $length\r\n\r\n";
        echo $error;
    }
}