#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include "errcodes.hpp"
# include <sys/socket.h>
# include <sys/types.h>
class Request
{
private:
	std::string							_method;
	std::string							_path;
	std::string							_version;
	std::map<std::string, std::string>	_data;
	std::string							_body;

	std::string							_responseHeader;
	std::pair<t_errcodes, std::string>	_responseBody;

	std::string	getFileType();
public:
	Request(int fd);
	~Request() {std::cerr << "Closing Request" << std::endl;};

	void	generateResponse();
	void	send(int fd);
};

#endif
