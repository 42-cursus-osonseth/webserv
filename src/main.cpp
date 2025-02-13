#include "../include/server.hpp"
#include <iostream>

int	main(int ac, char **av)
{
	Configuration	conf;

	if (ac > 2) {
		std::cout << "Error: Invalid arguments" << std::endl;
		exit(1);
	}
	conf.parseFile((ac == 1) ? "config/default.conf" : av[1]);
	try {
		Server	serv(conf);
		serv.start();
	} catch (const std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}
