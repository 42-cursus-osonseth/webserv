#include "includes/Server.hpp"

Server::Server(){}
Server::~Server(){}

void	Server::addPorts(const std::string &portStr)
{
	listenPorts.push_back(portStr);
}

void	Server::addServerNames(const std::string &serverName)
{
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

void	Server::addErrorPage(const std::pair<std::vector<int>, std::string> errorInfo)
{
	errorPages.insert(errorInfo);
}

void	Server::setLocation(const t_location &location)
{
	this->location = location;
}

t_location	&Server::getLocation()
{
	return this->location;
}