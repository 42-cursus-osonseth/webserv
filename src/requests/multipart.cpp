#include <request.hpp>

void Request::handleMultipart()
{
	size_t pos;
	updateBodyReadStatus();
	_tmpBody = _body;
	_body = "";

	while (1) {
		if (_clientRef.getState() == READING_BOUNDARY && !readingBoundary(pos))
			break;
		if (_clientRef.getState() == READING_MULTIPART_HEADER)
			readingMultipartHeader(pos);
		if (_clientRef.getState() == READING_MULTIPART_DATA && !readingMultipartData(pos))
			break;
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
bool Request::readingBoundary(size_t &pos)
{
	pos = _tmpBody.find(_clientRef.getBoundary());
	if (pos != std::string::npos && _tmpBody.substr(pos, _clientRef.getFinalBoundary().size()) == _clientRef.getFinalBoundary() && _clientRef.getBodyFullyRead())
		return false;
	if (pos == std::string::npos || _tmpBody.substr(_clientRef.getBoundary().size(), 2) != "\r\n")
		throw Request::ErrcodeException(BAD_REQUEST, *this);
	_tmpBody.erase(0, _clientRef.getBoundary().size() + 2);
	_clientRef.setState(READING_MULTIPART_HEADER);
	return true;
}
void Request::readingMultipartHeader(size_t &pos)
{
	pos = _tmpBody.find("\r\n\r\n");
	if (pos == std::string::npos)
		throw Request::ErrcodeException(BAD_REQUEST, *this);
	checkFilePresence(pos);
}
std::string getExtension(std::string filename)
{
	size_t pos = filename.find('.');
	size_t posend = filename.find('"');
	std::string extension = filename.substr(pos, posend - pos);
	return extension;
}
void Request::checkFilePresence(size_t &pos)
{
	std::string header = _tmpBody.substr(0, pos);
	pos = header.find("filename=");
	std::string filename = header.substr(pos + 9, 2);
	if (filename == "\"\"")
		goToNextBoundary(pos);
	else
	{
		
		filename = header.substr(pos + 10);
		_extension = getExtension (filename);
		goToData(pos);
	}
}
void Request::goToNextBoundary(size_t &pos)
{
	pos = _tmpBody.find(_clientRef.getBoundary());
	_tmpBody.erase(0, pos);
	_clientRef.setState(READING_BOUNDARY);
}
void Request::goToData(size_t &pos)
{
	pos = _tmpBody.find("\r\n\r\n");
	generateUniqueFilename();
	_tmpBody.erase(0, pos + 4);
	_clientRef.setState(READING_MULTIPART_DATA);
}
bool Request::readingMultipartData(size_t &pos)
{
	if (!_clientRef.getPartialBuffer().empty())
		prependPartialBuffer();
	pos = _tmpBody.find(_clientRef.getBoundary());
	if (pos == std::string::npos)
	{
		processIncompleteMultipartData();
		return false;
	}
	else
		processCompleteMultipartData(pos);
	return true;
}
void Request::processIncompleteMultipartData()
{
	std::string toSend = _tmpBody.size() > 100 ? _tmpBody.substr(0, _tmpBody.size() - 100) : "";
	_clientRef.setPartialBuffer(_tmpBody.size() > 100 ? _tmpBody.substr(_tmpBody.size() - 100) : "");
	if (!toSend.empty())
	{
		_body = toSend;
		cgiManager cgi(*this, _clientRef);
		cgi.execute(_clientRef);
	}
}
void Request::processCompleteMultipartData(size_t &pos)
{
	_body = _tmpBody.substr(0, pos - 2);
	_tmpBody.erase(0, pos);
	_clientRef.setState(READING_BOUNDARY);
	cgiManager cgi(*this, _clientRef);
	cgi.execute(_clientRef);
}