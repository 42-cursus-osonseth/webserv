#include "Server.hpp"

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