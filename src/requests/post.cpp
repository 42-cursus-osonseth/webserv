#include <request.hpp>

void Request::readRemainingBody()
{
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


void Request::postReq()
{
	getRessourcePath();
	_clientRef.setMime(_mime = get_mime(Utils::getExtension(_path)));
	_clientRef.setContentType(_data["Content-Type"]);

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
