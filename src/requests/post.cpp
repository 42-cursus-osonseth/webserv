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

void Request::readRemainingBody()
{
	std::cout << " ON RENTRE DANS READ_REMAINING_BODY" << std::endl;
	if (_clientRef.getIsChunk())
	{
		_body = getRequest();
		assemblagedebloc();
	}

	else
	{
		_body = getRequest();
		_clientRef.setBytesRead(_body.size() + _clientRef.getBytesRead());
		_clientRef.printClient();
		std::cout << std::string(30, '-') << std::endl;
		std::cout << _body << std::endl;
		std::cout << std::string(30, '-') << std::endl;
		// debugPrintBodyReadable(10);
		if (_clientRef.getBytesRead() < _clientRef.getContentLenght())
			_clientRef.setBobyFullyRead(false);
		else if (_clientRef.getBytesRead() == _clientRef.getContentLenght())
			_clientRef.setBobyFullyRead(true);
		else
			std::cerr << "ERROR lecture superieur au content length" << std::endl;
	}
}
void Request::assemblagedebloc()
{
	_clientRef.setBobyFullyRead(false);
	std::cout << std::string(30, '-') << std::endl;
	std::cout << "ON RENTRE DANS L'ASSEMBLAGE" << std::endl;
	std::cout << std::string(30, '-') << std::endl;
	std::cout << " ---------------------------------  CLIENT INFO  -----------------------------------" << std::endl;
	_clientRef.printClient();
	std::cout << std::string(30, '-') << std::endl;
	std::cout << " ---------------------------------  BODY RECU  -----------------------------------" << std::endl;
	std::cout << _body << std::endl;
	std::cout << std::string(30, '-') << std::endl;

	std::string tmpBody = _body;
	std::string result;

	if (!_clientRef.getPartialChunkSize().empty())
	{
		tmpBody = _clientRef.getPartialChunkSize() + tmpBody;
		_clientRef.setPartialChunkSize("");
	}
	while (1)
	{
		if (_clientRef.GetState() == READING_CHUNK_SIZE)
		{
			size_t pos = tmpBody.find("\r\n");
			if (pos == std::string::npos && tmpBody.size() > 6)
				std::cerr << "BLABLA BAD REQUEST" << std::endl;
			else if (pos == std::string::npos && tmpBody.size() < 7)
			{
				_clientRef.setPartialChunkSize(tmpBody);
				break;
			}
			std::string chunkSize = tmpBody.substr(0, pos);
			if (!Utils::isValidChunkSize(chunkSize))
				std::cerr << "BLABLA BAD REQUEST" << std::endl;

			_clientRef.setChunkSize(Utils::htoi(chunkSize));
			if (_clientRef.getChunkSize() == 0)
			{
				if (tmpBody.substr(0, 5) != "0\r\n\r\n")
					std::cerr << "BLABLA BAD REQUEST" << std::endl;
				_clientRef.setBobyFullyRead(true);
				break;
			}
			tmpBody.erase(0, chunkSize.size() + 2);
			_clientRef.setState(READING_CHUNK_DATA);
		}
		if (_clientRef.GetState() == READING_CHUNK_DATA)
		{
			std::string chunk = tmpBody.substr(0, _clientRef.getChunkSize());
			result += chunk;
			if (chunk.size() < _clientRef.getChunkSize())
			{
				size_t remaining = _clientRef.getChunkSize() - chunk.size();
				_clientRef.setChunkSize(remaining);
				break;
			}
			tmpBody.erase(0, chunk.size());
			if (tmpBody.size() < 2 || tmpBody.substr(0, 2) != "\r\n")
				std::cerr << "BLABLA BAD REQUEST" << std::endl;
			else
				tmpBody.erase(0, 2);
			_clientRef.setState(READING_CHUNK_SIZE);
		}
	}

	std::cout << std::string(30, '-') << std::endl;
	std::cout << "       RESULT = " << result.size() << std::endl;
	std::cout << std::string(30, '-') << std::endl;
	std::cout << result << std::endl;
	std::cout << std::string(30, '-') << std::endl;
	// std::string blockSize = _body.substr(0, _body.find("\r\n"));
	// std::string tmpBody = _body.substr(blockSize.size() + 2);
	// _clientRef.setChunkSize(Utils::htoi(blockSize));
	// size_t pos_end = std::min<size_t>(_clientRef.getChunkSize() - _clientRef.getCurrentChunkread(), BUFFER_SIZE);
	// if (pos_end > tmpBody.size())
	// 	_body = tmpBody;
	// else
	// {
	// 	tmpBody = tmpBody.substr(0, pos_end);
	// }
}

void Request::postReq()
{

	getRessourcePath();
	_mime = get_mime(Utils::getExtension(_path));
	std::cout << " ---------------------------------  DATA  -----------------------------------" << std::endl;
	for (std::map<std::string, std::string>::iterator it = _data.begin(); it != _data.end(); ++it)
		std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::string(30, '-') << std::endl;
	if (_clientRef.getIsChunk())
	{
		assemblagedebloc();
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

	// std::string filename = "file" + getDate();
	// std::cout << std::string(30, '-') << std::endl;
	// std::cout << "filename = " << filename << std::endl;
	// std::cout << std::string(30, '-') << std::endl;
	//--------------------------------------------------------------------------

	// debugPrintBodyReadable(10);
	//--------------------------------------------------------------------------
	// std::cout << "BODY SYZE = " << _body.size() << std::endl;

	// std::cout << std::string(30, '-') << std::endl;
	// std::cout << "METHOD = " << _method << " PATH = " << _fullPath << std::endl;
	// _clientRef.printClient();
	// std::cout << std::string(30, '-') << std::endl;

	if (_mime == "application/x-httpd-php" || _mime == "text/x-python")
		throw Request::CGIcalled();
}
void Request::debugPrintBodyReadable(int nbLines)
{
	std::istringstream stream(_body);
	std::string line;
	int count = 0;
	size_t headerSize = 0;

	std::cout << "=== Début du body ===" << std::endl;

	// On tente d’afficher les premières lignes lisibles
	while (std::getline(stream, line) && count < nbLines)
	{
		// Retirer le \r si présent
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		// On ne print que si la ligne est lisible (ASCII)
		bool isBinary = false;
		for (size_t i = 0; i < line.size(); ++i)
		{
			if ((unsigned char)line[i] < 32 && line[i] != '\r' && line[i] != '\n' && line[i] != '\t')
			{
				isBinary = true;
				break;
			}
		}
		if (isBinary)
			break;

		std::cout << line << std::endl;
		headerSize += line.size() + 2; // +2 pour \r\n
		count++;

		if (line.empty())
			break;
	}

	size_t remainingSize = _body.size() - headerSize;
	std::cout << "[" << remainingSize << " octets de données]" << std::endl;

	// On affiche le boundary de fin s’il est là
	std::string endBoundary = _body.substr(_body.size() > 80 ? _body.size() - 80 : 0);
	size_t pos = endBoundary.find("--");
	if (pos != std::string::npos)
		std::cout << endBoundary.substr(pos) << std::endl;
	else
		std::cout << "PAS DE BOUNDARY DE FIN" << std::endl;

	std::cout << "=== Fin du body ===" << std::endl;
}
