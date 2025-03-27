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

	class IHtmlErrorException : public std::exception {
	public:
		virtual const char	*what(Request &r) const throw() = 0;
	};

	class NotFoundException : public IHtmlErrorException {
	public:
		const char	*what(Request &r) const throw();
	};
	class ForbiddenException : public IHtmlErrorException {
	public:
		const char	*what(Request &r) const throw();
	};
	class NotImplementedException : public IHtmlErrorException {
	public:
		const char	*what(Request &r) const throw();
	};
	class InvalidRequestException : public IHtmlErrorException {
	public:
		const char	*what(Request &r) const throw();
	};
	class InternalServerErrorException : public IHtmlErrorException {
	public:
		const char	*what(Request &r) const throw();
	};

};

#endif
