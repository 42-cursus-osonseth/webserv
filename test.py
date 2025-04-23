#!/usr/bin/env python3


import socket

host = '127.0.0.1'
port = 8080
path = '/process/cgi/post/registration.py'
chunk_size = 128

# génère des données
data = ('Zz' * (1 * 1024 // 2)).encode()  # 32 Ko

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

# envoie les headers
request_headers = (
    f"POST {path} HTTP/1.1\r\n"
    f"Host: {host}:{port}\r\n"
    "Transfer-Encoding: chunked\r\n"
    "Content-Type: text/plain\r\n"
    "Connection: close\r\n"
    "\r\n"
)

s.sendall(request_headers.encode())

# envoie les chunks
i = 0
while i < len(data):
    chunk = data[i:i+chunk_size]
    chunk_len = hex(len(chunk))[2:].encode()
    s.sendall(chunk_len + b"\r\n" + chunk + b"\r\n")
    i += chunk_size

# fin du transfert
s.sendall(b"0\r\n\r\n")

# lit la réponse
# response = b""
# while True:
#     part = s.recv(4096)
#     if not part:
#         break
#     response += part

# print(response.decode(errors="ignore"))
s.close()
