#include "includes/Server.hpp"
#include <sys/epoll.h>
std::list<Server> Server::serversList;
Server::Server(): clientMaxBodySize(0), sockfd(-1) 
{
	server_names.clear();
	listenPorts.clear();
	locations.clear();
	errorPages.clear();
	index.clear();
	root.clear();
	hostAddress.clear();
	std::memset(&addr, 0, sizeof(addr));
}

Server::Server(const Server &src)
{
	*this = src;
}


Server &Server::operator=(const Server &rhs)
{
	if (this == &rhs)
		return *this;
	server_names = rhs.server_names;
	listenPorts = rhs.listenPorts;
	clientMaxBodySize = rhs.clientMaxBodySize;
	index = rhs.index;
	root = rhs.root;
	hostAddress = rhs.hostAddress;
	locations = rhs.locations;
	errorPages = rhs.errorPages;
	sockfd = rhs.sockfd;
	addr = rhs.addr;
	return *this;
}

Server::~Server(){}

void	Server::acceptConnection(int epfd, struct epoll_event ev)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int client_fd = accept(sockfd, (struct sockaddr *)&addr, &addrlen);
	if (client_fd == -1)
		throw std::runtime_error("accept failed");
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = client_fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
		throw std::runtime_error("epoll_ctl failed");
}

void	Server::initSocket()
{
	this->number_server = toString(findNumberHost());
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		throw std::runtime_error("Error: socket creation failed on server " + number_server);
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		close(sockfd);
		throw std::runtime_error("Error: setsockopt failed on server " + number_server);
	}
	addr.sin_family = AF_INET;
	if (!listenPorts.empty())
		addr.sin_port = htons(std::atoi(listenPorts[0].c_str()));
	else
		addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = inet_addr(hostAddress.c_str());
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
	{
		close(sockfd);
		throw std::runtime_error("Error: set socket to non-blocking mode failed on server " + number_server);
	}
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		close(sockfd);
		throw std::runtime_error("Error: bind failed on server " + number_server);
	}
	if (listen(sockfd, SOMAXCONN) == -1)
	{
		close(sockfd);
		throw std::runtime_error("Error: listen failed on server " + number_server);
	}
}

void	Server::closeSocket()
{
	if (sockfd != -1)
	{
		close(sockfd);
		sockfd = -1;
	}
}

template <typename T>
void printTabInline(const std::vector<T>& vec) {
    if (vec.empty()) {
        return;
    }

    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << toString(vec[i]);
        if (i < vec.size() - 1) { 
            std::cout << " "; 
        }
    }
}

void	Server::addPorts(const std::string &portStr)
{
	listenPorts.push_back(portStr);
}

void	Server::addServerNames(const std::string &serverName)
{
	if (std::find(server_names.begin(), server_names.end(), serverName) == server_names.end())
		server_names.push_back(serverName);
}

void	Server::setMaxBodySize(size_t value)
{
	clientMaxBodySize = value;
}

void	Server::setRoot(const std::string &root)
{
	this->root = root;
}

void	Server::setIndex(const std::string &index)
{
	this->index = index;
}

void	Server::addErrorPage(const std::pair<int, std::string> errorInfo)
{
	errorPages.insert(errorInfo);
}

void	Server::addLocation(const t_location &location)
{
	std::list<t_location>::iterator it;
	it = std::find(locations.begin(), locations.end(), location);
	if (it == locations.end())
		locations.insert(it, location);
}

void	Server::addServer(Server &server)
{
	Server::serversList.push_back(server);
}

void  	Server::printServer()
{
	Server host;
	int i = 1;
	for (std::list<Server>::const_iterator it = Server::serversList.begin(); it != Server::serversList.end();it++)
	{
		i = 1;
		host = *it;
		std::cout << CYAN << std::setw(63) << std::setfill('*') << '\n';
		if (host.server_names.size() > 0)
		{
			std::cout << '*' << std::setw(16) << std::setfill(' ') << "Server Name:\t" << host.server_names[0] << std::setw(17-host.server_names[0].size()) << std::setfill(' ') << '\n';
		}
		if (host.listenPorts.size() > 0)
		{
			std::cout << '*' << std::setw(16) << std::setfill(' ') << "Listen:\t";
			for (std::vector<std::string>::const_iterator it = host.listenPorts.begin(); it != host.listenPorts.end();it++)
			{
				std::cout << *it << '\t';
				if (it + 1 == host.listenPorts.end())
					std::cout  << '\n';
			}
		}
		std::cout << '*' << std::setw(16) << std::setfill(' ') << "Host:\t" << host.hostAddress << '\n';
		if (host.errorPages.size() > 0)
		{
			for (std::map<int, std::string>::const_iterator it = host.errorPages.begin(); it != host.errorPages.end();it++)
			{
				std::cout << '*' << std::setw(16) << std::setfill(' ') << "Error Pages:\t"; 
				std::cout << it->first << '-' << it->second << '\n';
			}
		}
		if (!host.index.empty())
		{
			std::cout << '*' << std::setw(16) << std::setfill(' ') << "Index:\t" << host.index  << '\n';
		}
		if (!host.root.empty())
		{
			std::cout << '*' << std::setw(16) << std::setfill(' ') << "Root:\t" << host.root << '\n';
		}
		if (host.clientMaxBodySize > 0)
		{
			std::cout << '*' << std::setw(16) << std::setfill(' ') << "Max Body Size:\t" << host.clientMaxBodySize  << '\n';
		}
		std::cout << std::setw(63) << std::setfill('*') << '\n' << RESET;
		for (std::list<t_location>::const_iterator it = host.locations.begin(); it != host.locations.end();it++)
		{
			std::cout << "\n\n";
			std::cout << YELLOW "Location numero: [" <<  i++ << "]\n" << RESET;
			host.printLocation(*it);
		}
		std::cout << "\n\n\n\n";
	}
}

void	Server::addHostAddress(const std::string &hostAddress)
{
	this->hostAddress = hostAddress;
}


void	Server::printLocation(const t_location &location)
{
	std::cout << MAGENTA << std::setw(42) << std::setfill('*') << '\n';
	std::cout << "Location:\t" << location.uri << '\n';
	std::cout << "Root:\t" << location.root << '\n';
	std::cout << "Index:\t" << location.index << '\n';
	std::cout << "Dir Listing:\t" << (location.dirListing ? "on" : "off") << '\n';
	if (location.redir.first != 0)
		std::cout << "Redirection:\t" << location.redir.first << " " << location.redir.second << '\n';
	std::cout << "Methods:\t";
	for (std::list<std::string>::const_iterator it = location.methods.begin(); it != location.methods.end();it++)
	{
		std::cout << *it << '\t';
	}
	std::cout << '\n';
	std::cout << "Uplaod:\t\t" << (location.upload ? "on" : "off") << '\n';
	std::cout << "Upload Path:\t" << location.uploadPath << '\n';
	std::cout << std::setw(42) << std::setfill('*') << '\n' << RESET;
}



void	Server::checkRequiredElements()
{
	if (listenPorts.empty())
	throw std::runtime_error("Error: Missing listen directive");
	else if (hostAddress.empty())
	throw std::runtime_error("Error: Missing host address");
}

// std::list<Server>	Server::findHost(Server	& server)
// {
// 		for (std::list<Server>::const_iterator it = serversList.begin(); it != serversList.end();it++)
// 		{
// 				if (*it == server)
// 					return (it);
// 			}
// 			return (it);
// }

int Server::findNumberHost()
{
	int i = 0;
	for (std::list<Server>::const_iterator it = serversList.begin(); it != serversList.end();it++)
	{
		if (it->hostAddress == this->hostAddress && it->listenPorts == this->listenPorts)
			return i;
		i++;
	}
	return -1;
}
// void	Server::removeServer(Server &server)
// {
	// 	serversList.remove(server);
	// }


std::string Server::get_errcode_string(t_errcodes e)
{
	return errorPages[e];
	// for (std::map<int,std::string>::const_iterator it = errorPages.begin(); it != errorPages.end(); ++it)
	// {
		
	// }
}

Server* Server::getInstance(const std::string& host, int port)
{
	(void)port;
    for (std::list<Server>::const_iterator it = serversList.begin(); it != serversList.end(); ++it) {
		
        if (it->hostAddress == host && std::find(it->listenPorts.begin(), it->listenPorts.end(), toString(port)) != it->listenPorts.end())
		{
            return const_cast<Server*>(&(*it));
        }
    }
    return NULL;
}

const int& Server::getSockfd() const
{
	return sockfd;
}

std::string Server::getHostAddress()
{
	return hostAddress;
}

const std::string& Server::getHostAddress() const 
{
	return hostAddress;
}

const std::list<t_location>& Server::getLocations() const 
{
	return locations;
}

const std::map<int, std::string>& Server::getErrorPages() const 
{
        return errorPages;
}

const std::vector<std::string>& Server::getServerNames() const 
{
	return server_names;
}

const std::vector<std::string>& Server::getListenPorts() const
{
	return listenPorts;
}

size_t Server::getClientMaxBodySize() const 
{
        return clientMaxBodySize;
}

const std::string& Server::getIndex() const
{
	return index;
}

const std::string& Server::getRoot() const
{
	return root;
}


std::list<Server>& Server::getServersList()
{
	return serversList;
}
