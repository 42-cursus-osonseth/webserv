#include <request.hpp>

void	Request::placeHolderErrorGen(t_errcodes err)
{
	_errcode = err;
	_responseBody = std::string("<!DOCTYPE html><html><h1>") + Utils::itos(err) + "</h1><p>" + get_errcode_string(err) + "</p></html>";
	generateHeader();
}

Request::ErrcodeException::ErrcodeException(t_errcodes errcode) : _errcode(errcode) {}

const char	*Request::ErrcodeException::what(Request &r) const throw()
{
	r._errcode = _errcode;
	r._path = "./pages/" + Utils::itos(_errcode) + ".html";
	try {
		r.getFileContent();
	} catch (const Request::ErrcodeException &e) {
		r.placeHolderErrorGen(_errcode);
	}
	r.generateHeader();
	return ((get_errcode_string(_errcode) + " (" + Utils::itos(_errcode) + ")").c_str());
}
