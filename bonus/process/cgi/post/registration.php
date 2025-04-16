#!/usr/bin/php
<?php

$input = stream_get_contents(STDIN);
parse_str($input, $_POST);

$prenom = $_POST['Prenom'] ?? '';
$nom = $_POST['Nom'] ?? '';


echo "HTTP/1.1 302 Found\r\n";
echo "Location: /index.html\r\n";
echo "Set-Cookie: Prenom=$prenom; Path=/\r\n";
echo "Set-Cookie: Nom=$nom; Path=/\r\n";
echo "Content-Length: 0\r\n\r\n";

?>
