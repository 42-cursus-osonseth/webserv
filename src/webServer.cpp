#include <webServer.hpp>
#include <request.hpp>
#include <signal.h>
int g_loop = 1;
webServer::webServer() : epfd(-1), nfds(0), ev(), events() {}
webServer::~webServer() {}
webServer::webServer(const webServer &src)
{
	*this = src;
}

webServer::webServer(std::list<Server> serversList) : epfd(-1), nfds(0), ev(), events(), serversList(serversList) {}
webServer &webServer::operator=(const webServer &rhs)
{
	if (this != &rhs)
	{
		this->epfd = rhs.epfd;
		this->nfds = rhs.nfds;
		this->ev = rhs.ev;
		std::copy(rhs.events, rhs.events + MAX_EVENTS, events);
		this->serversList = rhs.serversList;
	}
	return *this;
}

std::string errorString()
{
	int err = errno;
	return strerror(err);
}

bool webServer::isServer(int fd)
{
	for (std::vector<int>::iterator it = server_fds.begin(); it != server_fds.end(); it++)
	{
		if (*it == fd)
			return true;
	}
	return false;
}

bool webServer::isClient(int fd)
{
	for (std::vector<int>::iterator it = client_fds.begin(); it != client_fds.end(); it++)
	{
		if (*it == fd)
			return true;
	}
	return false;
}

void handle_signal(int signal)
{
	if (signal == SIGQUIT || signal == SIGINT || signal == SIGTERM || signal == SIGHUP || signal == SIGTSTP)
		g_loop = 0;
}

void webServer::initEpoll()
{
	epfd = epoll_create(1);
	if (epfd == -1)
		throw std::runtime_error("epoll_create failed");
	if (fcntl(epfd, F_SETFD, FD_CLOEXEC) == -1)
		throw std::runtime_error("set epfd to FD_CLOEXEC failed");
	memset(&ev, 0, sizeof(ev));
	memset(events, 0, sizeof(struct epoll_event) * MAX_EVENTS);
}

void webServer::setupServers()
{
	for (std::list<Server>::iterator it = serversList.begin(); it != serversList.end(); it++)
	{
		it->initSocket();
		for (size_t i = 0; i < it->getSockfds().size(); ++i)
		{
			ev.events = EPOLLIN;
			ev.data.fd = it->getSockfds()[i];
			if (epoll_ctl(epfd, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1)
				throw std::runtime_error("epoll_ctl failed");
			std::cout << GREEN << "Server fd added: " << ev.data.fd << RESET << std::endl;
			server_fds.push_back(ev.data.fd);
		}
	}
}

void webServer::closeWebServer()
{
	std::cout << "ON FERME LE SERVEUR" << std::endl;
	for (std::vector<int>::iterator it = server_fds.begin(); it != server_fds.end(); it++)
	{
		if (epoll_ctl(epfd, EPOLL_CTL_DEL, *it, NULL) == -1)
			throw std::runtime_error("epoll_ctl_del failed for server_fds " + errorString() + " on fd" + toString(*it));
	}
	for (std::list<Server>::iterator it = serversList.begin(); it != serversList.end(); it++)
	{
		it->closeSocket();
	}
	for (std::vector<int>::iterator it = client_fds.begin(); it != client_fds.end(); it++)
	{
		if (epoll_ctl(epfd, EPOLL_CTL_DEL, *it, NULL) == -1)
			throw std::runtime_error("epoll_ctl_del failed for client_fds");
		close(*it);
	}
	close(epfd);
}

void webServer::acceptConnection(int server_fd)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
	if (client_fd == -1)
		throw std::runtime_error("accept failed");
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1 || fcntl(client_fd, F_SETFD, FD_CLOEXEC) == -1)
	{
		close(client_fd);
		throw std::runtime_error("Error: set client socket to O_NONBLOCK or FD_CLOEXEC failed");
	}
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = client_fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
	{
		close(client_fd);
		throw std::runtime_error("epoll_ctl failed");
	}
	std::cout << YELLOW << "Client connected on fd: " << client_fd << RESET << std::endl;
	client_fds.push_back(client_fd);
	clients[client_fd] = client(client_fd);
}

void webServer::setSocketMode(int client_fd, int mode)
{
	memset(&ev, 0, sizeof(ev));
	if (mode == LEVEL_TRIGGERED)
		ev.events = EPOLLIN;
	else
		ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = client_fd;
	if (epoll_ctl(epfd, EPOLL_CTL_MOD, client_fd, &ev) == -1)
	{
		close(client_fd);
		throw std::runtime_error("epoll_ctl failed");
	}
}
void webServer::start()
{
	signal(SIGQUIT, handle_signal); // ctrl backslash
	signal(SIGINT, handle_signal);	// ctrl c
	signal(SIGTSTP, handle_signal); // ctrl z
	signal(SIGTERM, handle_signal); // si on kill notre pid avec kill -15 PID
	signal(SIGHUP, handle_signal);	// si on kill notre pid avec kill -1 PID
	try
	{
		initEpoll();
		setupServers();
		while (g_loop)
		{
			nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
			std::cerr << GREEN << "WebServer wait an event\n"
					  << RESET;
			// for (std::map<int, client>::iterator it = clients.begin(); it != clients.end(); ++it)
			// 	it->second.printClient();
			if (nfds == -1)
			{
				if (errno == EINTR)
					continue;
				throw std::runtime_error("epoll_wait failed");
			}
			for (int i = 0; i < nfds; ++i)
			{
				std::cout << " EVENT FD = " << events[i].data.fd << std::endl;
				if (!isServer(events[i].data.fd) && events[i].events & EPOLLIN)
				{
					try
					{
						std::cerr << YELLOW << "Client socket event: read data : " << events[i].data.fd << RESET << std::endl;
						Request req(events[i].data.fd, clients[events[i].data.fd]);
						clients[events[i].data.fd].getBodyFullyRead() ? setSocketMode(events[i].data.fd, EDGE_TRIGGERED) : setSocketMode(events[i].data.fd, LEVEL_TRIGGERED);
						req.send();
						
					}
					catch (const std::exception &e)
					{
							std::cerr << "client efface fd = " << events[i].data.fd << std::endl;
							clients.erase(events[i].data.fd);
							epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
							if (isClient(events[i].data.fd))
								client_fds.erase(std::remove(client_fds.begin(), client_fds.end(), events[i].data.fd), client_fds.end());
							close(events[i].data.fd);
							std::cerr << e.what() << std::endl;
					}
					// Client socket event: read data.
				}
				else if (events[i].events & EPOLLIN)
				{
					for (std::list<Server>::iterator it = serversList.begin(); it != serversList.end(); it++)
					{
						const std::vector<int> &sockfds = it->getSockfds();
						for (size_t j = 0; j < sockfds.size(); j++)
						{
							std::cerr << BLUE << server_fds[j] << RESET << '\n';
							if (events[i].data.fd == sockfds[j])
							{
								acceptConnection(sockfds[j]);
								break;
							}
						}
					}
				}
			}
		}
		closeWebServer();
	}
	catch (const std::exception &e)
	{
		closeWebServer();
		std::cerr << RED << e.what() << RESET << std::endl;
	}
}