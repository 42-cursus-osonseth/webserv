#!/usr/bin/env php
<?php

date_default_timezone_set("Europe/Paris"); // adapte la timezone si besoin

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

$length = strlen($html_body);

echo "HTTP/1.1 200 OK\r\n";
echo "Content-Type: text/html; charset=UTF-8\r\n";
echo "Content-Length: $length\r\n\r\n";
echo $html_body;
?>
