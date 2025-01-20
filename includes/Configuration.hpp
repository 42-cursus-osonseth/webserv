/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekherbo <mekherbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 16:44:14 by mekherbo          #+#    #+#             */
/*   Updated: 2025/01/20 03:05:45 by mekherbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <string>
#include <map>
#include "colors.hpp"
#include "Server.hpp"
#include "utils.hpp"
class Configuration
{
	private:
		static int nbServer;
		static std::ifstream infile;
		static int nbPort;
		static bool	insideServer;
	public:
		Configuration();
		~Configuration();
		static void parseFile(const std::string &filename);
		static bool	handleToken(const std::string &token);
		static bool	handleServer();
		static void	parseBlock();
		static void	chooseDirectives(const std::string &line);
		static void	parseLocation(const std::string &line);
		static void	parseCgi(const std::string &line);
		static void parsePorts(const std::string &line, Server &server);
		static void parseServerName(const std::string &line, Server &server);
		static void parseErrorPage(const std::string &line, Server &server);
		static void parseMaxClients(const std::string &line, Server &server);
		class	BraceNotClosedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

		class	NoserverFoundException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

		class MissingSemicolonException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};



#endif