#include <request.hpp>

void Request::generateUniqueFilename()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000000 + tp.tv_usec;
	std::ostringstream oss;
	oss << ms;
	std::string filename = "Webserv_" + oss.str();
	_clientRef.setFilename(filename);
}
void Request::initClientData()
{
	size_t pos = _data["Content-Type"].find(";");
	_clientRef.setContentType(_data["Content-Type"].substr(0, pos));
	std::istringstream iss(_data["Content-Length"]);
	ssize_t len;
	iss >> len;
	_clientRef.setContentLength(len);
	_clientRef.setBytesRead(_body.size() + _clientRef.getBytesRead());
	pos = _data["Content-Type"].find("=");
	if (pos != std::string::npos)
		_clientRef.setBoundary("--" + _data["Content-Type"].substr(pos + 1));
}
void Request::readRemainingBody()
{
	// std::cout << " ON RENTRE DANS READ_REMAINING_BODY" << std::endl;
	if (_clientRef.getIsChunk())
	{
		_body = getRequest();
		ChunkedBodyAssembler();
	}

	else
	{
		_body = getRequest();
		_clientRef.setBytesRead(_body.size() + _clientRef.getBytesRead());
		if (_clientRef.getBytesRead() < _clientRef.getContentLenght())
			_clientRef.setBobyFullyRead(false);
		else if (_clientRef.getBytesRead() == _clientRef.getContentLenght())
			_clientRef.setBobyFullyRead(true);
		else
			std::cerr << "ERROR lecture superieur au content length" << std::endl;
	}
}
void Request::postReq()
{
	getRessourcePath();
	_clientRef.setMime(_mime = get_mime(Utils::getExtension(_path)));
	_clientRef.setContentType(_data["Content-Type"]);
	std::cout << " ---------------------------------  DATA  -----------------------------------" << std::endl;
	for (std::map<std::string, std::string>::iterator it = _data.begin(); it != _data.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::string(30, '-') << std::endl;
	std::cout << std::string(30, '-') << std::endl;
	_clientRef.printClient();
	std::cout << std::string(30, '-') << std::endl;
	if (_clientRef.getIsChunk())
	{
		_clientRef.setBobyFullyRead(false);
		generateUniqueFilename();
		ChunkedBodyAssembler();
	}

	else
	{
		initClientData();


	
		std::cout << " COUCOU ON ENTRE" << std::endl;
		std::cout << std::string(30, '-') << std::endl;
		_clientRef.printClient();
		std::cout << " BODY = " << std::endl;
		std::cout << _body << std::endl;
		std::cout << std::string(30, '-') << std::endl;
		// if (_data["Content-Type"] != "application/x-www-form-urlencoded")
		// 	generateUniqueFilename();
		// if (_clientRef.getBytesRead() < _clientRef.getContentLenght())
		// 	_clientRef.setBobyFullyRead(false);
		// else if (_clientRef.getBytesRead() == _clientRef.getContentLenght())
		// 	_clientRef.setBobyFullyRead(true);
		// else
		// 	std::cerr << "ERROR lecture superieur au content length" << std::endl;
	}

	if (_mime == "application/x-httpd-php" || _mime == "text/x-python")
		throw Request::CGIcalled();
}
