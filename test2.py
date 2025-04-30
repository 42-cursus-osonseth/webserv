#!/usr/bin/env python3

import socket
import os

host = '127.0.0.1'
port = 8080
path = '/process/cgi/post/upload-cookies.py'
chunk_size = 8192
file_path = '/home/max/Téléchargements/pute.png'

# Création de la socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

# Envoi des headers
request_headers = (
    f"POST {path} HTTP/1.1\r\n"
    f"Host: {host}:{port}\r\n"
    "Transfer-Encoding: chunked\r\n"
    "Content-Type: application/octet-stream\r\n"
    "Connection: close\r\n"
    "\r\n"
)

s.sendall(request_headers.encode())

# Envoi du corps en chunks
with open(file_path, "rb") as f:
    while True:
        chunk = f.read(chunk_size)
        if not chunk:
            break
        chunk_len = f"{len(chunk):X}".encode()  # taille en hexadécimal
        s.sendall(chunk_len + b"\r\n" + chunk + b"\r\n")

# Fin du transfert
s.sendall(b"0\r\n\r\n")

s.close()
