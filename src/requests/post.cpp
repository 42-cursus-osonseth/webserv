#include <request.hpp>

void	Request::postReq()
{
	getRessourcePath();
	_mime = get_mime(Utils::getExtension(_path));
	getBody();
	if(_mime == "application/x-httpd-php")
		throw Request::CGIcalled();
}
// void	Request::getFileContent()
// {
// 	std::stringstream	ress;
// 	std::ifstream		file;

// 	file.open(_path.c_str());
// 	if (!file.is_open())
// 		throw Request::ErrcodeException(INTERNAL_SERVER_ERROR, *this);
// 	ress << file.rdbuf();
// 	_responseBody = ress.str();
// 	file.close();
// }

// void	Request::getBody()
// {
// 	if (access(_path.c_str(), F_OK))
// 		throw Request::ErrcodeException(NOT_FOUND, *this);
// 	else if (access(_path.c_str(), F_OK | R_OK))
// 		throw Request::ErrcodeException(FORBIDDEN, *this);
// 	getFileContent();
// 	_errcode = OK;
// }

// void	Request::getReq()
// {
// 	getRessourcePath();
// 	if(_path == "./bonus/process")
// 	{
// 		generateSetCookieHeader();
// 		return;
// 	}
// 	getBody();
// 	_mime = get_mime(Utils::getExtension(_path));
// 	generateHeader();
// }