#include "../include/request.hpp"
#include "../include/utils.hpp"
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <sstream>

static std::pair<t_errcodes, std::string>	get_body(std::string filename)
{
	std::stringstream					ress;
	std::pair<t_errcodes, std::string>	res;
	size_t								mark = filename.find("?");
	std::ifstream						stream;

	if (mark != std::string::npos)
		filename = filename.substr(0, mark);
	if (Utils::pathIsDir(filename)) {
		filename += "/index.html"; // A ajuster Config
	}
	stream.open(filename);
	if (!stream.is_open()) {
		res.first = NOT_FOUND;
		res.second = get_body(std::string("./pages/404.html")).second; // Ouvrir la page 404 du configfile
	} else {
		ress << stream.rdbuf();
		res.second = ress.str();
		res.first = OK;
	}
	return res;
}

std::string	Request::getFileType()
{
	std::vector<std::string> file_types = Utils::split(_data["Accept"], ",");
	int	i = 0;

	if (!Utils::fileExists("." + _path)) { // Ajouter le path config
		std::cerr << "File does not exist" << std::endl;
		return ("text/html"); // utruculu
	}
	if (file_types.size() == 0) {
		std::cerr << "Request chelou" << std::endl;
		return ("text/html");
	}
	while (i < file_types.size()) {
		std::cout << "Testestest: " << file_types[i] << std::endl;
		if (file_types[i] == "*/*") {
			std::string	ext = Utils::getExtension(_path);
			if (ext == "") {
				file_types[i] = "text/html";
				break;
			}
			else if (ext == "png") {
				file_types[i] = "images/png";
				break;
			} else {
				file_types[i] = "text/" + ext;
				break;
			}
		}
		if (Utils::getExtension(_path) == file_types[i].substr(file_types[i].find("/") + 1)) {
			std::cout << "Extension is: " << file_types[i] << std::endl;
			break ;
		}
		++i;
	}
	if (i != file_types.size())
		return (file_types[i]);
	return ("text/html");
}

Request::Request(int fd)
{
	char						buffer[4096] = {0}; // Body size ici?
	int							n = read(fd, buffer, sizeof(buffer));

	// std::cerr << "Receiving: " << buffer << std::endl << "------------" << std::endl;
	std::vector<std::string>	lines = Utils::split(buffer, "\r\n");
	std::istringstream	request_line(lines[0]);
	request_line >> _method >> _path >> _version;
	std::cout << "Generating request for: " << _path << std::endl;
	// if (_path == "/favicon.ico")
	// 	_path = "/favicon.png";
	int	i = 1;
	if (lines[0].empty())
		return ;
	while (!lines[i].empty()) {
		size_t	sep_pos = lines[i].find(":");
		if (sep_pos != std::string::npos) {
			std::string	field_name = lines[i].substr(0, sep_pos);
			std::string	field_value = lines[i].substr(sep_pos + 2);
			_data[field_name] = field_value;
		}
		i++;
	}
}

void	Request::generateResponse()
{
	std::string	file_type = getFileType();

	_responseBody = get_body("." + _path); // Penser à utiliser le path de config
	_responseHeader = "HTTP/1.1 " + std::to_string(_responseBody.first) + " " + errcode_strings.at(_responseBody.first) + "\r\n";
	_responseHeader += "Server: Webserv\r\n"; // Config?
	_responseHeader += generate_time();
	_responseHeader += "Content-Length: " + std::to_string(_responseBody.second.length())  + "\r\n";
	_responseHeader += "Content-Type: " + file_type + "\r\n";
	_responseHeader += "Cache-Control: no-store\r\n\r\n";
}	

void	Request::send(int fd)
{
	generateResponse();
	if (::send(fd, _responseHeader.c_str(), _responseHeader.length(), MSG_NOSIGNAL) < 0) {
		perror("send");
	}
	std::cout << "Sent: " << _responseHeader << std::endl;
	if (::send(fd, _responseBody.second.c_str(), _responseBody.second.length(), MSG_NOSIGNAL) < 0) {
		perror("send");
		// throw
	}
	if (isprint(_responseBody.second.c_str()[0]))
	std::cout << "Sent: " << _responseBody.first << " " << _responseBody.second << std::endl;
	std::cout << "Response sent" << std::endl;
}
