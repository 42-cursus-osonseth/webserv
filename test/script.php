#!/usr/bin/php
<?php
$input = trim(fgets(STDIN));  

parse_str($input, $data);  

$prenom = $data['Prenom'] ?? '';  
$nom = $data['Nom'] ?? '';       

echo "HTTP/1.1 302 Found\n";  // Code de redirection
echo "Location: /registration.html\n";  // Redirection vers redirection.html
echo "Set-Cookie: Prenom=$prenom; Path=/\n";
echo "Set-Cookie: Nom=$nom; Path=/\n";
echo "Content-Length: 0\r\n\r\n";

?>
