#include <request.hpp>

const char	*Request::NotFoundException::what(Request &r) const throw()
{
	r._errcode = NOT_FOUND;
	r._path = "./pages/404.html";
	try {
		r.getFileContent();
	} catch (const std::exception &e) {
		return e.what();
	}
	r.generateHeader();
	return ("The ressource has been found but can't be accessed (403)");
}

const char	*Request::ForbiddenException::what(Request &r) const throw()
{
	r._errcode = FORBIDDEN;
	r._path = "./pages/403.html";
	try {
		r.getFileContent();
	} catch (const std::exception &e) {
		return e.what();
	}
	r.generateHeader();
	return ("The ressource has been found but can't be accessed (403)");
}

const char	*Request::NotImplementedException::what(Request &r) const throw()
{
	r.notImplementedPlaceholder();
	// generateHeader();
	return ("This functionnality has not been implemented (501)");
}

const char	*Request::InvalidRequestException::what(Request &r) const throw()
{
	r.generateHeader();
	return ("The request isn't valid (400)");
}

const char	*Request::InternalServerErrorException::what(Request &r) const throw()
{
	r.generateHeader();
	return ("Internal server error (500)");
}