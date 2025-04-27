#include <request.hpp>
#include <utils.hpp>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <sstream>

const std::string &Request::getterBody() const
{
	return _body;
}
const std::string &Request::getterQuery() const
{
	return _query;
}
const std::string &Request::getPath() const
{
	return _path;
}
const std::string &Request::getMethod() const
{
	return _method;
}
int Request::getFd() const
{
	return _fd;
}
void Request::getQuerry()
{
	size_t sep_pos = _path.find('?');
	_query = _path.substr(sep_pos + 1);
	_path = _path.substr(0, sep_pos);
}

void Request::isolateBody(std::string &fullRequest)
{
	// std::cerr << "Isolating body on: " << fullRequest << std::endl;

	size_t sep = fullRequest.find("\r\n\r\n");
	if (sep != std::string::npos)
	{
		_body = fullRequest.substr(sep + 4);
		fullRequest = fullRequest.substr(0, sep);
	}
	// std::cerr << "Body isolation managed" << std::endl;
}

std::string Request::getRequest()
{
	char buffer[BUFFER_SIZE] = {0};
	ssize_t n;
	std::string fullRequest;

	n = recv(_fd, buffer, sizeof(buffer), 0);
	if (n == 0)
		throw Request::Disconnected();
	if (n > 0)
		fullRequest.append(buffer, n);
	else
	{
		std::cerr << "Encountered error when reading from socket: " << Utils::itos(_fd) << std::endl;
		throw Request::ErrcodeException(INTERNAL_SERVER_ERROR, *this);
	}
	return fullRequest;
}

void Request::parseRequest()
{
	std::string fullRequest = getRequest();
	isolateBody(fullRequest);
	std::vector<std::string> lines = Utils::split(fullRequest.c_str(), "\r\n");
	std::istringstream request_line(lines[0]);
	request_line >> _method >> _path >> _version;
	_path.find('?') != std::string::npos ? getQuerry() : void(); // si trouve un ? separe le path de la querry string
	long unsigned int i = 1;
	if (_method.empty() || _path.empty() || _version.empty())
	{ // Checks sur le format
		_method = "GET";
		_path = "/";
		_version = "HTTP/1.1";
		throw Request::ErrcodeException(BAD_REQUEST, *this);
	}
	while (i < lines.size() && !lines[i].empty())
	{
		size_t sep_pos = lines[i].find(":");
		if (sep_pos != std::string::npos)
		{
			std::string field_name = lines[i].substr(0, sep_pos);
			std::string field_value = lines[i].substr(sep_pos + 2);
			_data[field_name] = field_value;
		}
		i++;
	}
	if (_data.find("Transfer-Encoding") != _data.end() && _data["Transfer-Encoding"] == "chunked")
		_clientRef.setIsChunck(true);
	try
	{
		_data.at("Host");
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "No host found in the header file" << std::endl;
		throw Request::ErrcodeException(BAD_REQUEST, *this);
	}
}

std::string Request::getHost()
{
	size_t pos = _data["Host"].find(":");

	if (pos == std::string::npos)
	{
		return _data["Host"];
	}
	return _data["Host"].substr(0, pos);
}

int Request::getPort()
{
	size_t pos = _data["Host"].find(":");
	int res;

	if (pos == std::string::npos)
		return 80;
	std::istringstream(_data["Host"].substr(pos + 1)) >> res;
	return res;
}

void Request::generateResponse()
{
	std::cerr << "Looking up: " << getHost() << "@" << Utils::itos(getPort()) << std::endl;
	_matchingServer = Server::getServersList().front().getInstance(getHost(), getPort());
	if (!_matchingServer)
	{
		std::cout << "No matching server" << std::endl;
		throw Request::ErrcodeException(INTERNAL_SERVER_ERROR, *this); // [TBR]
	}
	if (_method == "GET")
		getReq();
	else if (_method == "POST")
		postReq();
	else if (_method == "DELETE")
		deleteReq();
	else
		throw Request::ErrcodeException(NOT_IMPLEMENTED, *this);
}

Request::Request(int fd, client &client) : _fd(fd), _processDir("/process"), _clientRef(client)
{
	std::cerr << "Generating a new Request" << std::endl;
	try
	{
		_matchingServer = NULL;
		if (_clientRef.getBodyFullyRead())
		{
			parseRequest();
			generateResponse();
		}
		else
		{	
			readRemainingBody();
			throw Request::CGIcalled();
		}
	}
	catch (const CGIcalled &e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const Disconnected &e)
	{
		throw e;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Encountered exception while treating request: " << e.what() << std::endl;
	}
}

void Request::send()
{

	if (!_errcode)
	{
		std::cerr << "Can't respond to this request" << std::endl;
	}
	else
	{
		if (_clientRef.getMime() == "text/x-python" && _clientRef.getContentType() == "multipart/form-data")
			return;
		else if (_clientRef.getMime() == "text/x-python")
		{
			cgiManager cgi(*this, _clientRef);
			cgi.execute(_clientRef);
			return;
		}
		else if (_clientRef.getMime() == "application/x-httpd-php")
		{
			cgiManager cgi(*this, _clientRef);
			cgi.execute(_clientRef);
			return;
		}

		if (::send(_fd, _responseHeader.c_str(), _responseHeader.length(), MSG_NOSIGNAL) < 0)
			perror("send");
		if (::send(_fd, _responseBody.c_str(), _responseBody.length(), MSG_NOSIGNAL) < 0)
			perror("send");
	}
}

void Request::generateHeader()
{
	_responseHeader = _version + ' ' + Utils::itos(_errcode) + ' ' + get_errcode_string(_errcode) + "\r\n";
	std::cout << _version;
	std::cout << ' ' + Utils::itos(_errcode) + ' ' + get_errcode_string(_errcode) + "\r\n";
	if (_matchingServer)
		_responseHeader += "Server: " + _matchingServer->getServerNames()[0] + "\r\n"; // Config file dependent _matchingServer.getServerNames()
	else
		_responseHeader += "Server: Error server\r\n";
	_responseHeader += Utils::time_string();
	_responseHeader += "Content-Length: " + Utils::itos(_responseBody.size()) + "\r\n";
	_responseHeader += "Content-Type: " + _mime + "\r\n";
	_responseHeader += "Connection: keep-alive\r\n";
	_responseHeader += "Cache-Control: no-store\r\n\r\n";
}

void Request::dump(void)
{
	std::cout << "-- Dumping request for " << _path << " at " << _data["Host"] << " --" << std::endl;
	std::cout << _responseHeader << _responseBody << std::endl;
}
