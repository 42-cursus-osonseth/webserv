/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Configuration.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mekherbo <mekherbo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 16:44:14 by mekherbo          #+#    #+#             */
/*   Updated: 2025/02/22 04:53:36 by mekherbo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include "library.hpp"
#include "Server.hpp"
#include "checkFiles.hpp"
class Configuration
{
	private:
		static int nbServer;
		static std::ifstream infile;
		static int nbPort;
		static bool	insideServer;
		static bool	locationFlag;
		static bool cgiFlag;
		static int	lineNbr;
		static t_location currentLocation;
	public:
		Configuration();
		~Configuration();
		static void parseFile(const std::string &filename);
		static bool	handleToken(const std::string &token);
		static bool	handleServer();
		static void	parseBlock();
		static bool	chooseDirectives(const std::string &line, Server &server);
		static bool	chooseLocationDirectives(const std::string &line);
		static void	parseLocation(const std::string &line);
		static void	parseCgi(const std::string &line);
		static void parsePorts(const std::string &line, Server &server);
		static void parseServerName(const std::string &line, Server &server);
		static void parseErrorPage(const std::string &line, Server &server);
		static void parseMaxClients(const std::string &line, Server &server);
		static void	parseRoot(const std::string &line, Server &server);
		static void	parseIndex(const std::string &line, Server &server);
		static void	parseHostAddress(const std::string &line, Server &server);

		// fill Location
		static void	parseRootLocation(const std::string &line);
		static void	parseMethods(const std::string &line);
		static void	parseRedirection(const std::vector<std::string> &lineSplitted);
		static void	parseDirListing(const std::string &line);
		static void	parseDirFile(const std::string &line);
		static void parseIndexLocation(const std::string &line);
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
		
		class EmptyPortsException : public std::exception
		{
			public:
				virtual const char* what() const	throw();
		};

		class InvalidPortsException : public std::exception
		{
			public:
				virtual const char* what() const	throw();
		};
		class EmptyServerNameException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class InvalidDnsException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class EmptyPageErrorException: public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class InvalidPageErrorException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class InvalidMaxBodySizeException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class MissingRootException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class MissingIndexException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class LocationArgsException : public std::runtime_error
		{
			public:
			LocationArgsException(const std::string& message) : std::runtime_error(message) {}
		};

};

#endif