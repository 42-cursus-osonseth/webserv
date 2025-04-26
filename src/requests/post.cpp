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
	_clientRef.setBytesRead(0);
	pos = _data["Content-Type"].find("=");
	if (pos != std::string::npos)
	{
		_clientRef.setBoundary("--" + _data["Content-Type"].substr(pos + 1));
		_clientRef.setFinalBoundary(_clientRef.getBoundary() + "--");
	}
	_clientRef.setState(READING_BOUNDARY);
}

void Request::readRemainingBody()
{
	std::cout << " ON RENTRE DANS READ_REMAINING_BODY" << std::endl;
	if (_clientRef.getIsChunk())
	{
		_body = getRequest();
		ChunkedBodyAssembler();
	}

	else
	{
		_body = getRequest();
		handleMultipart();
	}
}
void Request::updateBodyReadStatus()
{
	_clientRef.setBytesRead(_body.size() + _clientRef.getBytesRead());
	if (_clientRef.getBytesRead() < _clientRef.getContentLenght())
		_clientRef.setBobyFullyRead(false);
	else if (_clientRef.getBytesRead() == _clientRef.getContentLenght())
		_clientRef.setBobyFullyRead(true);
}
bool Request::readingBoundary(size_t &pos, std::string &tmpBody)
{
	pos = tmpBody.find(_clientRef.getBoundary());
	if (pos != std::string::npos && tmpBody.substr(pos, _clientRef.getFinalBoundary().size()) == _clientRef.getFinalBoundary() && _clientRef.getBodyFullyRead())
		return false;
	if (pos == std::string::npos || tmpBody.substr(_clientRef.getBoundary().size(), 2) != "\r\n")
		throw Request::ErrcodeException(BAD_REQUEST, *this);
	tmpBody.erase(0, _clientRef.getBoundary().size() + 2);
	_clientRef.setState(READING_MULTIPART_HEADER);
	return true;
}
void Request::readingMultipartHeader(size_t &pos, std::string &tmpBody)
{
	pos = tmpBody.find("\r\n\r\n");
	if (pos == std::string::npos)
		throw Request::ErrcodeException(BAD_REQUEST, *this);
	checkFilePresence(pos, tmpBody);
}
void Request::checkFilePresence(size_t &pos, std::string &tmpBody)
{
	std::string header = tmpBody.substr(0, pos);
	pos = header.find("filename=");
	std::string filename = header.substr(pos + 9, 2);
	if (filename == "\"\"")
		goToNextBoundary(pos, tmpBody);
	else
		goToData(pos, tmpBody);
}
void Request::goToNextBoundary(size_t &pos, std::string &tmpBody)
{
	pos = tmpBody.find(_clientRef.getBoundary());
	tmpBody.erase(0, pos);
	_clientRef.setState(READING_BOUNDARY);
}
void Request::goToData(size_t &pos, std::string &tmpBody)
{
	pos = tmpBody.find("\r\n\r\n");
	generateUniqueFilename();
	tmpBody.erase(0, pos + 4);
	_clientRef.setState(READING_MULTIPART_DATA);
}

bool Request::readingMultipartData(size_t &pos, std::string &tmpBody)
{
	if (!_clientRef.getPartialBuffer().empty())
	{
		tmpBody = _clientRef.getPartialBuffer() + tmpBody;
		_clientRef.setPartialBuffer("");
	}
	pos = tmpBody.find(_clientRef.getBoundary());
	if (pos == std::string::npos)
	{
		processIncompleteMultipartData(tmpBody);
		return false;
	}
	else
		processCompleteMultipartData(pos, tmpBody);
	return true;
}
void Request::processIncompleteMultipartData(std::string &tmpBody)
{
	std::string toSend = tmpBody.size() > 100 ? tmpBody.substr(0, tmpBody.size() - 100) : "";
	_clientRef.setPartialBuffer(tmpBody.size() > 100 ? tmpBody.substr(tmpBody.size() - 100) : "");
	if (!toSend.empty())
	{
		_body = toSend;
		cgiManager c(*this, _clientRef);
		c.execute(_clientRef);
	}
}
void Request::processCompleteMultipartData(size_t &pos, std::string &tmpBody)
{
	_body = tmpBody.substr(0, pos - 2);
	tmpBody.erase(0, pos);
	_clientRef.setState(READING_BOUNDARY);
	cgiManager c(*this, _clientRef);
	c.execute(_clientRef);
}

void Request::handleMultipart()
{
	size_t pos;
	updateBodyReadStatus();
	std::string tmpBody = _body;
	_body = "";

	while (1)
	{
		if (_clientRef.getState() == READING_BOUNDARY && !readingBoundary(pos, tmpBody))
			break;
		if (_clientRef.getState() == READING_MULTIPART_HEADER)
			readingMultipartHeader(pos, tmpBody);
		if (_clientRef.getState() == READING_MULTIPART_DATA && !readingMultipartData(pos, tmpBody))
			break;
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
	// std::cout << std::string(30, '-') << std::endl;
	// std::cout << "INFO CLIENT" << std::endl;
	// _clientRef.printClient();

	if (_clientRef.getIsChunk())
	{
		_clientRef.setBobyFullyRead(false);
		generateUniqueFilename();
		ChunkedBodyAssembler();
	}
	else if (_clientRef.getContentType() == "application/x-www-form-urlencoded")
		throw Request::CGIcalled();

	else
	{
		initClientData();
		handleMultipart();
		if (_mime == "application/x-httpd-php" || _mime == "text/x-python")
			throw Request::CGIcalled();
	}
}
