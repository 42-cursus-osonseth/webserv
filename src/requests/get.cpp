#include <request.hpp>
#include <utils.hpp>
#include <mime.hpp>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <sstream>

void	Request::getFileContent()
{
	std::stringstream	ress;
	std::ifstream		file;

	file.open(_path.c_str());
	if (!file.is_open())
		throw Request::ErrcodeException(INTERNAL_SERVER_ERROR);
	ress << file.rdbuf();
	_responseBody = ress.str();
	file.close();
}

void	Request::getBody()
{
	if (access(_path.c_str(), F_OK))
		throw Request::ErrcodeException(NOT_FOUND);
	else if (access(_path.c_str(), F_OK | R_OK))
		throw Request::ErrcodeException(FORBIDDEN);
	getFileContent();
	_errcode = OK;
}

void	Request::getReq()
{
	getRessourcePath();
	getBody();
	generateHeader();
}
