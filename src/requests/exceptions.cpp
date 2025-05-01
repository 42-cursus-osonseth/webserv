#include <request.hpp>
#include <sys/types.h>
#include <dirent.h>

void	Request::placeHolderErrorGen(t_errcodes err)
{
	_errcode = err;
	_responseBody = std::string("<!DOCTYPE html><html><h1>") + Utils::itos(err) + "</h1><p>" + get_errcode_string(err) + "</p></html>";
	_version = "HTTP/1.1";
	_mime = "text/html";
	generateHeader();
}

Request::ErrcodeException::ErrcodeException(t_errcodes errcode, Request &r) : _errcode(errcode), _r(r)
{
	_errstring = get_errcode_string(_errcode) + " (" + Utils::itos(_errcode) + ")";
}

void	Request::ErrcodeException::fix() const
{
	_r._errcode = _errcode;
	if (_r._matchingServer) {
		try {
			_r._path = _r._matchingServer->getErrorPages().at(_errcode);
			_r.getFileContent();
		} catch (const Request::ErrcodeException &e) {
			_r.placeHolderErrorGen(_errcode);
		}
	} else {
		_r.placeHolderErrorGen(_errcode);
	}
	_r.generateHeader();
}

const char	*Request::ErrcodeException::what() const throw()
{
	fix();
	return (_errstring.c_str());
}

Request::AutoIndexHandle::AutoIndexHandle(Request &r) : r(r)
{
	r._errcode = OK; // Attention: Peut renvoyer INTERNAL_SERVER_ERROR en cas de fail de dir
	r._mime = "text/html";
}

const char	*Request::AutoIndexHandle::what() const throw()
{
	r._responseBody = std::string("Autoindex WIP");
	std::cerr << "Generating autoindex page for: " << r._path << std::endl;
	r._responseBody =	"<!DOCTYPE html>\n"
						"<html>\n"
						"\t<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
						"\t<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css\">\n"
						"<style>\n"
						"\tbody {\n"
						"\t\tfont-family: sans-serif;\n"
						"\t\tbackground-color: #f8f8f8;\n"
						"\t\tmargin: 0;\n"
						"\t\tpadding: 20px\n;"
						"\t}"
						"\t.directory-listing\n"
						"\t{\n"
						"\t\tpadding: 20px;\n"
						"\t\tmax-width: 600px;\n"
						"\t\tmargin: 20px auto;\n"
						"\t}\n"
						"\th1\n"
						"\t{\n"
						"\t\tfont-size: 1.5em;\n"
						"\t\tmargin-bottom: 20px;\n"
						"\t\tcolor: #333;\n"
						"\t}\n"
						"\tul\n"
						"\t{"
						"\t\tlist-style: none;\n"
						"\t\tpadding: 0;\n"
						"\t}\n"
						"\tli {\n"
						"\t\tmargin-bottom: 10px;\n"
						"\t}\n"
						"\ta\n" 
						"\t{\n"
						"\t\ttext-decoration: none;\n"
						"\t\tcolor: #007bff;\n"
						"\t\tdisplay: inline-block;\n"
						"\t\tpadding: 5px 10px;\n"
						"\t\tborder-radius: 3px;\n"
						"\t}\n"
						"\ta:hover\n"
						"\t{\n"
						"\t\tbackground-color: #f0f0f0;\n"
						"\t}\n"
						"\ti\n"
						"\t{\n"
						"\t\tmargin-right: 5px;\n"
						"\t\tcolor: #666;\n"
						"\t}\n"
						"</style>\n"
						"<title>Index</title>\n"
						"</head>\n"
						"<body>\n"
						"\t<div class=\"directory-listing\">\n"
						"\t\t<h1> Index of /www/</h1>\n"
						"\t\t<ul>\n";
	std::cerr << "Autoindexing " << r._path << std::endl;
	DIR	*dir = opendir(r._path.c_str());
	if (!dir) {
		try {
			throw Request::ErrcodeException(INTERNAL_SERVER_ERROR, r);
		} catch (std::exception &e) {
			return (e.what());
		}
	}
	struct dirent	*ent;
	while ((ent = readdir(dir))) {
		r._responseBody += "<li><a href=\"" + std::string(ent->d_name) + "\"><i class=\"";
		if (ent->d_type == DT_DIR)
			r._responseBody += "fas fa-folder";
		else
			r._responseBody += "far fa-file";
		r._responseBody += "\"></i>" + std::string(ent->d_name) + "</a></li>";
	}
	r._responseBody += "</ul></div><h2 style=\"text-align=center;\">This page is autoindexed</h2></body></html>";
	r.generateHeader();
	return ("Autoindex has been called");
}

Request::Redirection::Redirection(Request &r, std::pair<int, std::string> info) : _r(r), _redir(info) {}

const char	*Request::Redirection::what() const throw()
{
	_r._responseBody = "";
	_r._responseHeader = "HTTP/1.1 " + Utils::itos(_redir.first) + " " + get_errcode_string((t_errcodes)_redir.first) + "\r\n";
	_r._responseHeader += "Location: " + _redir.second + "\r\n";
	_r._responseHeader += "Content-Length: 0\r\n";
	_r._responseHeader += "Connection: keep-alive\r\n";
	_r._responseHeader += "Server: " + _r._matchingServer->getServerNames()[0] + "\r\n\r\n";
	return ("Some redirection has been used");
}
