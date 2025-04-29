#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <errcodes.hpp>
#include <mime.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctime>
#include <sys/time.h>
#include "utils.hpp"
#include "Server.hpp"
#include "cgiManager.hpp"
#include "client.hpp"

#define BUFFER_SIZE 50000

class Request
{
private:
	int _fd;
	std::string _processDir;
	client &_clientRef;

	std::string _root;
	std::string _method;
	std::string _path;
	std::string	_fullPath;
	std::string _version;
	std::string _mime;
	std::string _body;
	std::string _tmpBody;
	std::string _query;
	std::string _chunk;
	std::string _extension;
	std::string _uploadPath;

	std::map<std::string, std::string> _data;
	
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
	std::string getHost();
	int getPort();
	std::string replaceRoot(const std::string &, const std::string &);
	void getQuerry();
	bool isProcessPath();

	// generateResponse main subfunctions
	void getReq();
	void postReq();
	void deleteReq();

	// POST 

	void readRemainingBody();
	void generateUniqueFilename();
	void ChunkedBodyAssembler();
	void prependPartialBuffer();
	bool readChunkSize();
	bool readChunkData();
	bool findCRLF(size_t &pos);
	bool getAndValidChunkSize(size_t &pos);
	bool checkAndPrepareForTrailingCRLF();
	void initClientData();
	void handleMultipart();
	void updateBodyReadStatus();
	bool readingBoundary(size_t &pos);
	void readingMultipartHeader(size_t &pos);
	bool readingMultipartData(size_t &pos);
	void checkFilePresence(size_t &pos);
	void goToNextBoundary(size_t &pos);
	void goToData(size_t &pos);
	void processIncompleteMultipartData();
	void processCompleteMultipartData(size_t &pos);

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
	Request(int fd, client &client);
	~Request() { std::cerr << "Closing Request" << std::endl
						   << std::endl; };

	void send();
	void dump();
	const std::string &getterBody() const;
	const std::string &getterQuery() const;
	const std::string &getPath() const;
	const std::string &getMethod() const;
	int getFd() const;
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
		public:
		const char	*what() const throw() { return "CGI has been called"; }
	};
};

#endif
