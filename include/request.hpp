#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <errcodes.hpp>
# include <mime.hpp>
# include <sys/socket.h>
# include <sys/types.h>
# include "utils.hpp"
# include "../includes/Server.hpp"

class Request
{
private:
	int									_fd;
	std::string							_method;
	std::string							_path;
	std::string							_version;
	std::map<std::string, std::string>	_data;
	std::string							_body;

	t_errcodes							_errcode;

	std::string							_responseHeader;
	std::string							_responseBody;

	Server								*_matchingServer;

	// Basic functions for the constructor
	void	parseRequest();
	void	generateResponse();
	void	generateHeader();

	// generateResponse main subfunctions
	void	getReq();
	void	postReq();
	void	deleteReq();

	// GET related
	void	getFileContent();
	void	getBody();

	// Placeholder errcodes
	void	notImplementedPlaceholder();

	void	findMatchingServer();
	void	getRessourcePath();
public:
	Request(int fd);
	~Request() {std::cerr << "Closing Request" << std::endl;};

	void	send();
	void	dump();

	class NotFoundException : public std::exception {
		const char	*what(Request &r) const throw();
	};
	class ForbiddenException : public std::exception {
		const char	*what(Request &r) const throw();
	};
	class NotImplementedException : public std::exception {
		const char	*what(Request &r) const throw();
	};
	class InvalidRequestException : public std::exception {
		const char	*what(Request &r) const throw();
	};
	class InternalServerErrorException : public std::exception {
	public:
		const char	*what(Request &r) const throw();
	};

};

#endif
