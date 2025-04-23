#!/usr/bin/env python3
import sys
import os
import urllib.parse

input_data = sys.stdin.buffer.read()
sys.stdout.write(f"CONTENT_TYPE: {os.environ.get('CONTENT_TYPE')}\n")
sys.stdout.write(f"TRANSFER_ENCODING: {os.environ.get('TRANSFER_ENCODING')}\n")

if os.environ.get("TRANSFER_ENCODING", "") == "chunked":
     filename = os.environ.get("FILENAME")
     if filename:
        script_dir = os.path.dirname(__file__)
        upload_path = os.path.abspath(os.path.join(script_dir, "../../upload"))
        file_path = os.path.join(upload_path, filename)
        with open(file_path, "ab") as f:
           f.write(input_data)
     sys.stdout.write("LE ELSE IF DU SCRIPT EST EXEC")
     sys.stdout.write(f"FILENAME = {filename}\n")
     sys.stdout.write(f"FILE PATH = {file_path}\n")

# Vérifier le type de contenu
elif os.environ.get("CONTENT_TYPE") == "application/x-www-form-urlencoded" and os.environ.get("TRANSFER_ENCODING", "") == "":
    post_data = urllib.parse.parse_qs(input_data)
    prenom = post_data.get('Prenom', [''])[0]
    nom = post_data.get('Nom', [''])[0]

    # Écrire la réponse HTTP correctement formatée
    sys.stdout.write("HTTP/1.1 302 Found\r\n")
    sys.stdout.write("Location: /index.html\r\n")
    sys.stdout.write(f"Set-Cookie: Prenom={prenom}; Path=/\r\n")
    sys.stdout.write(f"Set-Cookie: Nom={nom}; Path=/\r\n")
    sys.stdout.write("Content-Length: 0\r\n\r\n")


