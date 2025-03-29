#include "includes/Configuration.hpp"
#include "includes/webServer.hpp"
#include "includes/errcodes.hpp"
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
	Server* s = Server::getInstance("127.0.0.1", 8080); 
	std::cout << "error code 404\t" << s->get_errcode_string(NOT_FOUND) << '\n';
	webServer nginx(Server::getServersList());
	try
	{
		nginx.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << RED <<  e.what() << '\n';
	}
	return (0);
}