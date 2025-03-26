#ifndef SERVER_HPP
# define SERVER_HPP

# include "configuration.hpp"
# include "request.hpp"

# include <sys/epoll.h>	// Pour utiliser epoll
# include <fcntl.h>		// Pour manipuler les flags des fichiers
# include <exception>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>

# define MAX_EVENTS 10
# define QUEUE_SIZE 5

class	Server {
private:
	int					_fd;
	int					_epfd;
	int					_nfds;
	struct epoll_event	_event;
	struct epoll_event	_events[MAX_EVENTS];
	struct sockaddr_in	_server_addr;

public:
	Server();
	Server(const Configuration &conf);
	~Server();
	void	start(void);

	class ServerInitFail : public std::exception {
		const char	*what() const throw();
	};
};

#endif
