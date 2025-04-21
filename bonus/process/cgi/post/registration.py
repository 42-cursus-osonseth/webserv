#!/usr/bin/env python3
import sys
import os
import urllib.parse

input_data = sys.stdin.read()

# Vérifier le type de contenu
if os.environ.get("CONTENT_TYPE") == "application/x-www-form-urlencoded":
    post_data = urllib.parse.parse_qs(input_data)
    prenom = post_data.get('Prenom', [''])[0]
    nom = post_data.get('Nom', [''])[0]

    # Écrire la réponse HTTP correctement formatée
    sys.stdout.write("HTTP/1.1 302 Found\r\n")
    sys.stdout.write("Location: /index.html\r\n")
    sys.stdout.write(f"Set-Cookie: Prenom={prenom}; Path=/\r\n")
    sys.stdout.write(f"Set-Cookie: Nom={nom}; Path=/\r\n")
    sys.stdout.write("Content-Length: 0\r\n\r\n")
