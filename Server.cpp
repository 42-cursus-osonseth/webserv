#include "includes/Server.hpp"

std::list<Server> Server::serversList;
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


void	Server::addHostAddress(const std::string &hostAddress)
{
	this->hostAddress = hostAddress;
}

void  	Server::printServer()
{
	Server host = Server::serversList.front(); 
	std::cout << CYAN << std::setw(42) << std::setfill('*') << '\n';
	std::cout << '*' << std::setw(15) << std::setfill(' ') << "Server\t" << host.host << std::setw(21-host.index.size()) << std::setfill(' ') << '*' << '\n';
	std::cout << std::setw(42) << std::setfill('*') << '\n' << RESET;
}
// std::list<Server>	Server::findHost(Server	& server)
// {
// 	for (std::list<Server>::const_iterator it = serversList.begin(); it != serversList.end();it++)
// 	{
// 		if (*it == server)
// 			return (it);
// 	}
// 	return (it);
// }

// void	Server::removeServer(Server &server)
// {
// 	serversList.remove(server);
// }