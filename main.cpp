#include "includes/Configuration.hpp"

int main(int ac, char **av)
{
	if (ac > 2)
	{
		std::cerr << SUPA_RED << "Error: too more args\n";
		return 1;
	}
	try
	{
			Configuration::parseFile(ac == 1 ? "conf/Default.conf" : av[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << SUPA_RED << e.what() << '\n' << RESET;
	}
	Server::printServer();
	std::string path = "/www";	
	if (isValidDir(formatPath(path)))
		std::cerr << "dir is valid\n";
	else
		std::cerr << "dir is not valid\n";
	std::list<Server>&slist = Server::getServersList();
	for (std::list<Server>::iterator it = slist.begin(); it != slist.end(); it++)
	{
		it->initSocket();
		std::cout << "Server: " << it->getHostAddress() << '\n';
	}
	for (std::list<Server>::iterator it = slist.begin(); it != slist.end(); it++)
	{
		it->closeSocket();
		std::cout << "Server: " << it->getHostAddress() << '\n';
	}
	return (0);
}