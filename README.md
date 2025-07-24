# 🌐 Webserv

L’objectif est d’implémenter un **serveur HTTPS** en C++ compatible avec la norme **RFC 2616 (HTTP/1.1)**.

---

## ⚙️ Fonctionnalités principales

- 📦 **Support complet du protocole HTTP/1.1**
  - Méthodes : `GET`, `POST`, `DELETE`
  - Redirections (`301`, `302`)
  - Gestion des codes d’erreur avec pages personnalisées
  - Configuration flexible via fichiers `.conf`
  - Serveurs virtuels (vhosts)
  - Autoindex des répertoires

- 🔒 **HTTPS obligatoire**
  - Connexions sécurisées via SSL/TLS
  - Certificats auto-signés
  - Communication compatible avec navigateur et `curl -k`

- 🗂️ **Serveur de fichiers statiques**
  - Réponse de fichiers HTML, CSS, images, etc.
  - Détection automatique des types MIME

- ⚙️ **CGI**
  - Support des scripts exécutables (ex. : Python)
  - Communication entre le serveur et le programme CGI

- 📥 **Upload de fichiers**
  - Support de `multipart/form-data`
  - Lecture en streaming, gestion des requêtes `chunked`

- ⚡ **Performances**
  - Utilisation de `epoll` pour la gestion des connexions clients
  - Serveur non-bloquant et monothread (event-driven)

## ➕ Bonus

- 🍪 **Cookies & gestion de session**
  - Lecture et écriture d'en-têtes `Set-Cookie` / `Cookie`
  - Exemple simple de session persistante avec identifiant

- 🔄 **CGI multiples**
  - Support de plusieurs exécutables CGI avec gestion des extensions

