#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <string>
#include <map>
// #include "colors.hpp"
// #include "utils.hpp"

class Configuration
{
	private:
		static int				nbServer;
		static std::ifstream	infile;
		int						nbPort;
		static bool				insideServer;
	public:
		Configuration();
		~Configuration();
		static void	parseFile(const std::string &filename);
		static bool	handleToken(const std::string &token);
		static bool	handleServer();
};

#endif
