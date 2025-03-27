#include <request.hpp>
#include <utils.hpp>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <sstream>

void	Request::parseRequest()
{
	char	buffer[4096] = {0};
	read(_fd, buffer, sizeof(buffer)); // A single read might not be enough

	std::vector<std::string>	lines = Utils::split(buffer, "\r\n");
	std::istringstream	request_line(lines[0]);
	request_line >> _method >> _path >> _version;
	int	i = 1;
	if (_method.empty() || _path.empty() || _version.empty())
		throw Request::InvalidRequestException();
	while (!lines[i].empty()) {
		size_t	sep_pos = lines[i].find(":");
		if (sep_pos != std::string::npos) {
			std::string	field_name = lines[i].substr(0, sep_pos);
			std::string	field_value = lines[i].substr(sep_pos + 2);
			_data[field_name] = field_value;
		}
		i++;
	}
	// Need to get the request body
}

void	Request::findMatchingServer(void)
{
	std::list<Server>	servers = Server::getServersList();

	for (std::list<Server>::iterator it = servers.begin(); it != servers.end(); it++) {
			if (1) { // Check is server is the corresponding one
				_matchingServer = &(*it);
				break ;
			}
	}
}

void	Request::generateResponse()
{
	findMatchingServer();
	if (!_matchingServer) {
		std::cout << "No matching server" << std::endl;
		throw Request::InternalServerErrorException();
	}
	if (_method == "GET") getReq();
	else if (_method == "POST") postReq();
	else if (_method == "DELETE") deleteReq();
	else throw Request::NotImplementedException();
}

Request::Request(int fd) : _fd(fd)
{
	try {
		parseRequest();
		generateResponse();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

void	Request::send()
{
	if (!_errcode) {
		std::cerr << "Can't respond to this request" << std::endl;
	} else {
		std::cerr << "WIP" << std::endl;
		// if (::send(fd, _responseHeader.c_str(), _responseHeader.length(), MSG_NOSIGNAL) < 0)
		// 	perror("send");
		// if (::send(fd, _responseBody.c_str(), _responseBody.length(), MSG_NOSIGNAL) < 0)
		// 	perror("send");	
	}
}

void	Request::getRessourcePath()
{
	std::string	fullPath = "./pages" + _path; // TEMP utiliser ["location"] du config
	size_t	mark = fullPath.find("?");

	if (mark != std::string::npos)
		fullPath = fullPath.substr(0, mark);
	else if (Utils::pathIsDir(fullPath))
		fullPath += "/index.html"; // Selon config ["index"]
	_path = fullPath;
}

void	Request::generateHeader()
{
	_responseHeader = _version + ' ' + Utils::itos(_errcode) + ' ' + get_errcode_string(_errcode) + "\r\n";
	_responseHeader += "Server: Webserv\r\n"; // Config file dependent _matchingServer.getServerNames()
	_responseHeader += Utils::time_string();
	_responseHeader += "Content-Length: " + Utils::itos(_responseBody.size()) + "\r\n";
	_responseHeader += "Content-Type: " + get_mime(Utils::getExtension(_path)) + "\r\n";
	_responseHeader += "Cache-Control: no-store\r\n\r\n";
}

void	Request::dump(void)
{
	std::cout << "-- Dumping request for " << _path << " at " << _data["Host"] << " --" << std::endl;
	std::cout << _responseHeader << std::endl;
	std::cout << _responseBody << std::endl;
}