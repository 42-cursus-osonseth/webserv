#include "includes/Configuration.hpp"

int main(int ac, char **av)
{
	try
	{
		if (ac == 2)
			Configuration::parseFile(av[1]);
		else
			Configuration::parseFile("conf/Default.conf");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return (0);
}