#include <Configuration.hpp>
#include <request.hpp>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int	main(int ac, char **av)
{
	Configuration::parseFile(ac != 2 ? "conf/Default.conf" : av[1]);

	Server::printServer();
	while (1) {
		std::string	entry;

		std::cout << "$> ";
		std::cin >> entry;
		if (entry == "stop")
			break ;
		else if (entry == "req") {
			std::cout << "Please enter a request name: ";
			std::cin >> entry;
			try {
				int fd;

				entry = "test_request/" + entry + ".http";
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
