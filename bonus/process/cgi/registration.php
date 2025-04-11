#!/usr/bin/env php
<?php
// Récupérer les données du formulaire
$prenom = isset($_POST['Prenom']) ? $_POST['Prenom'] : '';
$nom= isset($_POST['Nom']) ? $_POST['Nom'] : '';

// Affichage des headers HTTP attendus par ton CGI
echo "Content-Type: text/html\r\n";
echo "Set-Cookie: prenom=" . urlencode($prenom) . "\r\n";
echo "Set-Cookie: nom=" . urlencode($nom) . "\r\n";
echo "Location: /registration.html\r\n";
echo "\r\n";
?>
