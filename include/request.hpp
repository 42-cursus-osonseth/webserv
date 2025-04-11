#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <errcodes.hpp>
#include <mime.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include "utils.hpp"
#include "Server.hpp"
#include "cgiManager.hpp"

class Request
{
private:
	int _fd;
	std::string _method;
	std::string _path;
	std::string _version;
	std::map<std::string, std::string> _data;
	std::string _body;
	std::string _mime;
	std::string _query;
	std::string _processDir;
	std::string _root;
	std::string	_fullPath;

	t_errcodes _errcode;

	std::string _responseHeader;
	std::string _responseBody;

	Server *_matchingServer;

	// Basic functions for the constructor
	std::string getRequest();
	void isolateBody(std::string &);
	void parseRequest();
	void generateResponse();
	void generateHeader();
	void generateSetCookieHeader();
	std::string getHost();
	int getPort();
	std::string replaceRoot(const std::string &, const std::string &);
	void getQuerry();
	bool isProcessPath(const std::string &root, const std::string &fullPath);

	// generateResponse main subfunctions
	void getReq();
	void postReq();
	void deleteReq();

	// GET related
	void getFileContent();
	void getBody();

	// Placeholder errcodes
	void notImplementedPlaceholder();
	void internalServerErrorPlaceholder();
	void placeHolderErrorGen(t_errcodes);

	// Looking up stuff for the response
	const t_location *findMatchingLocation();
	void findMatchingServer();
	void getRessourcePath();

public:
	Request(int fd);
	~Request() { std::cerr << "Closing Request" << std::endl
						   << std::endl; };

	void send();
	void dump();

	class ErrcodeException : public std::exception
	{
	private:
		t_errcodes _errcode;
		std::string _errstring;
		Request &_r;
		ErrcodeException();
		void fix() const;

	public:
		ErrcodeException(t_errcodes, Request &);
		~ErrcodeException() throw() {}
		const char *what() const throw();
	};
	class AutoIndexHandle : public std::exception
	{
	private:
		Request &r;
		AutoIndexHandle();

	public:
		AutoIndexHandle(Request &);
		~AutoIndexHandle() throw() {};
		const char *what() const throw();
	};

	class Disconnected : public std::exception
	{
		const char *what() const throw() { return "Disconnected"; }
	};
	class Redirection : public std::exception
	{
	private:
		Request &_r;
		std::pair<int, std::string> _redir;
		Redirection();

	public:
		Redirection(Request &r, std::pair<int, std::string>);
		~Redirection() throw() {}
		const char *what() const throw();
	};
	class CGIcalled : public std::exception
	{
		const char	*what() const throw() { return "CGI has been called"; }
	};
};

#endif
