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
		std::cerr << SUPA_RED << e.what() << '\n';
	}
	
	return (0);
}