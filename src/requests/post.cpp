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
bool Request::findCRLF(size_t &pos)
{
	pos = _tmpBody.find("\r\n");
	if (pos == std::string::npos && _tmpBody.size() > 6)
		throw Request::ErrcodeException(BAD_REQUEST, *this);
	else if (pos == std::string::npos && _tmpBody.size() < 7)
	{
		_clientRef.setPartialChunkSize(_tmpBody);
		return false;
	}
	return true;
}
bool Request::checkAndPrepareForTrailingCRLF()
{
	if (_tmpBody.size() == 1)
	{
		_clientRef.setPartialChunkSize(_tmpBody);
		_clientRef.setReadCRLFfirst(true);
		_clientRef.setState(READING_CHUNK_SIZE);
		return false;
	}
	if (_tmpBody.size() == 0)
	{
		_clientRef.setReadCRLFfirst(true);
		_clientRef.setState(READING_CHUNK_SIZE);
		return false;
	}
	return true;
}
bool Request::getAndValidChunkSize(size_t &pos)
{
	std::string chunkSize = _tmpBody.substr(0, pos);
	if (!Utils::isValidChunkSize(chunkSize))
		throw Request::ErrcodeException(BAD_REQUEST, *this);
	_clientRef.setChunkSize(Utils::htoi(chunkSize));
	if (_clientRef.getChunkSize() == 0)
	{
		if (_tmpBody.substr(0, 5) != "0\r\n\r\n")
			throw Request::ErrcodeException(BAD_REQUEST, *this);
		_clientRef.setBobyFullyRead(true);
		return false;
	}
	_tmpBody.erase(0, chunkSize.size() + 2);
	return true;
}
bool Request::readChunkSize()
{
	if (_clientRef.getReadCRLFfirst())
	{
		if (_tmpBody.substr(0, 2) != "\r\n")
			throw Request::ErrcodeException(BAD_REQUEST, *this);
		_tmpBody.erase(0, 2);
		_clientRef.setReadCRLFfirst(false);
	}
	size_t pos;
	if (!findCRLF(pos))
		return false;
	if (!getAndValidChunkSize(pos))
		return false;
	_clientRef.setState(READING_CHUNK_DATA);
	return true;
}
bool Request::readChunkData()
{
	_chunk = _tmpBody.substr(0, _clientRef.getChunkSize());
	_body += _chunk;
	if (_chunk.size() < _clientRef.getChunkSize())
	{
		size_t remaining = _clientRef.getChunkSize() - _chunk.size();
		_clientRef.setChunkSize(remaining);
		return false;
	}
	_tmpBody.erase(0, _chunk.size());
	if (!checkAndPrepareForTrailingCRLF())
		return false;
	if (_tmpBody.substr(0, 2) != "\r\n")
		throw Request::ErrcodeException(BAD_REQUEST, *this);
	else
		_tmpBody.erase(0, 2);
	_clientRef.setState(READING_CHUNK_SIZE);
	return true;
}
void Request::prependPartialChunkSize()
{
	_tmpBody = _clientRef.getPartialChunkSize() + _tmpBody;
	_clientRef.setPartialChunkSize("");
}

void Request::ChunkedBodyAssembler()
{
	_tmpBody = _body;
	_body.clear();

	if (!_clientRef.getPartialChunkSize().empty())
		prependPartialChunkSize();
	while (1)
	{
		if (_clientRef.GetState() == READING_CHUNK_SIZE && !readChunkSize())
			break;
		if (_clientRef.GetState() == READING_CHUNK_DATA && !readChunkData())
			break;
	}
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
	std::cout << " ---------------------------------  DATA  -----------------------------------" << std::endl;
	for (std::map<std::string, std::string>::iterator it = _data.begin(); it != _data.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::string(30, '-') << std::endl;
	if (_clientRef.getIsChunk())
	{
		_clientRef.setBobyFullyRead(false);
		generateUniqueFilename();
		ChunkedBodyAssembler();
	}

	else
	{
		std::istringstream iss(_data["Content-Length"]);
		ssize_t len;
		iss >> len;
		_clientRef.setContentLength(len);
		_clientRef.setBytesRead(_body.size() + _clientRef.getBytesRead());
		_clientRef.setContentType(_data["Content-Type"]);
		if (_data["Content-Type"] != "application/x-www-form-urlencoded")
			generateUniqueFilename();
		if (_clientRef.getBytesRead() < _clientRef.getContentLenght())
			_clientRef.setBobyFullyRead(false);
		else if (_clientRef.getBytesRead() == _clientRef.getContentLenght())
			_clientRef.setBobyFullyRead(true);
		else
			std::cerr << "ERROR lecture superieur au content length" << std::endl;
	}

	if (_mime == "application/x-httpd-php" || _mime == "text/x-python")
		throw Request::CGIcalled();
}
