#include "includes/Configuration.hpp"
#include <request.hpp>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int	main(int ac, char **av)
{
	Configuration::parseFile(ac != 2 ? "conf/Default.conf" : av[1]);

	while (1) {
		std::string	entry;

		std::cin >> entry;
		if (entry == "stop")
			break ;
		else if (entry == "newrequest") {
			std::cin >> entry;
			try {
				int fd;

				if ((fd = open(entry.c_str(), O_RDONLY)) < 0)
					throw std::runtime_error("File could not be loaded");
				Request	r(fd);
				r.dump();
			} catch (const std::exception &e) {
				std::cout << e.what() << std::endl;
			}
		}
		else
			std::cout << "Invalid command" << std::endl;
	}
}
