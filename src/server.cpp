#include <server.hpp>
#include <cerrno>
#include <cstring>
#include <errcodes.hpp>
#include <mime.hpp>

Server::Server()
{
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0) {
		perror("socket creation failed");
		throw (ServerInitFail());
	}
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port = htons(8080); // Le port doit être dans le config file ?

	if (bind(_fd, (struct sockaddr *)(&_server_addr), sizeof(_server_addr)) < 0) {
		perror("bind failed");
		close(_fd);
		throw (ServerInitFail());
	}
	if (listen(_fd, QUEUE_SIZE) < 0) {
		perror("listen failed");
		close(_fd);
		throw (ServerInitFail());
	}
	_epfd = epoll_create1(0);
	if (_epfd < 0) {
		perror("epoll_create1 failed");
		close(_fd);
		throw (ServerInitFail());
	}

	_event.events = EPOLLIN;	// Surveiller les événements en lecture (nouvelles connexions)
	_event.data.fd = _fd;		// Associer le descripteur du socket serveur
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, _fd, &_event) < 0) {
		perror("epoll_ctl failed");
		close(_fd);
		close(_epfd);
		throw (ServerInitFail());
	}
}

Server::Server(const Configuration &conf)
{
	(void)conf;
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0) {
		perror("socket creation failed");
		throw (ServerInitFail());
	}
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port = htons(8080); // Le port doit être dans le config file ?

	if (bind(_fd, (struct sockaddr *)(&_server_addr), sizeof(_server_addr)) < 0) {
		perror("bind failed");
		close(_fd);
		throw (ServerInitFail());
	}
	if (listen(_fd, QUEUE_SIZE) < 0) {
		perror("listen failed");
		close(_fd);
		throw (ServerInitFail());
	}
	_epfd = epoll_create1(0);
	if (_epfd < 0) {
		perror("epoll_create1 failed");
		close(_fd);
		throw (ServerInitFail());
	}

	_event.events = EPOLLIN;	// Surveiller les événements en lecture (nouvelles connexions)
	_event.data.fd = _fd;		// Associer le descripteur du socket serveur
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, _fd, &_event) < 0) {
		perror("epoll_ctl failed");
		close(_fd);
		close(_epfd);
		throw (ServerInitFail());
	}
}

Server::~Server()
{
	close(_fd);
	close(_epfd);
	std::cout << "Server successfully closed" << std::endl;
}

void	Server::start()
{
	while (1) {
		// Attendre les événements sur les descripteurs surveillés
		_nfds = epoll_wait(_epfd, _events, MAX_EVENTS, -1); // Bloque jusqu'à ce qu'un événement survienne
		if (_nfds < 0) {
			perror("epoll_wait failed");
			break;
		}
		int	client_fd;

		// Parcourir les descripteurs prêts
		for (int i = 0; i < _nfds; i++) {
			if (_events[i].data.fd == _fd) { 
				// Un événement sur le socket serveur signifie qu'une nouvelle connexion est arrivée
				client_fd = accept(_fd, NULL, NULL); // Accepter la connexion
				if (client_fd < 0) {
					perror("accept failed");
					continue;
				}
				// Configurer le descripteur du client pour les événements en lecture (avec mode edge-triggered)
				_event.events = EPOLLIN | EPOLLET;  // EPOLLET = mode edge-triggered
				_event.data.fd = client_fd;
				if (epoll_ctl(_epfd, EPOLL_CTL_ADD, client_fd, &_event) < 0) {
					perror("epoll_ctl (add client_fd) failed");
					close(client_fd);
				}
			} else {
				try {
					Request	req(_events[i].data.fd);
					req.send(_events[i].data.fd);
				} catch (std::exception &e) {
					epoll_ctl(_epfd, EPOLL_CTL_DEL, _events[i].data.fd, NULL); // Remove from epoll
					close(_events[i].data.fd);
				}
			}
		}
	}
}

const char	*Server::ServerInitFail::what() const throw()
{
	return ("Error when initializing server");
}