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
	// std::cout << " ON RENTRE DANS READ_REMAINING_BODY" << std::endl;
	if (_clientRef.getIsChunk())
	{
		_body = getRequest();
		ChunkedBodyAssembler();
	}

	else
	{
		_body = getRequest();
	}
}
void Request::postReq()
{
	getRessourcePath();
	_clientRef.setMime(_mime = get_mime(Utils::getExtension(_path)));
	_clientRef.setContentType(_data["Content-Type"]);

	// std::cout << " ---------------------------------  DATA  -----------------------------------" << std::endl;
	// for (std::map<std::string, std::string>::iterator it = _data.begin(); it != _data.end(); ++it)
	// 	std::cout << it->first << ": " << it->second << std::endl;
	// std::cout << std::string(30, '-') << std::endl;

	if (_clientRef.getIsChunk())
	{
		_clientRef.setBobyFullyRead(false);
		generateUniqueFilename();
		ChunkedBodyAssembler();
	}

	else
	{
		size_t pos;
		std::string tmpBody = _body;
		initClientData();
		_clientRef.setBytesRead(tmpBody.size() + _clientRef.getBytesRead());
		if (_clientRef.getBytesRead() < _clientRef.getContentLenght())
			_clientRef.setBobyFullyRead(false);

		// std::cout << std::string(30, '-') << std::endl;
		// std::cout << " INFO CLIENT AVANT = " << std::endl;
		// _clientRef.printClient();
		// std::cout << std::string(30, '-') << std::endl;

		while (1)
		{
			std::cout << "TOUR DE BOUCLE" << std::endl;
			std::cout << "CLIENT INFO" << std::endl;
			_clientRef.printClient();
			if (_clientRef.getState() == READING_BOUNDARY)
			{
				std::cout << "ON VA LIRE LE BOUNDARY" << std::endl;
				pos = tmpBody.find(_clientRef.getBoundary());
				// on check si c'est le bounday de fin et qu on a lu toutes les donnes, si c'est le cas on sort de la boucle
				if (pos != std::string::npos && tmpBody.substr(pos, _clientRef.getFinalBoundary().size()) == _clientRef.getFinalBoundary())
				{
					std::cout << "ON ENTRE DANS LE CHECK BOUNDARY DE FIN ET ON BREAK" << std::endl;
					if (_clientRef.getBytesRead() == _clientRef.getContentLenght())
						_clientRef.setBobyFullyRead(true);
					break;
				}
				if (pos == std::string::npos || tmpBody.substr(_clientRef.getBoundary().size(), 2) != "\r\n")
					throw Request::ErrcodeException(BAD_REQUEST, *this);
				tmpBody.erase(0, _clientRef.getBoundary().size() + 2);
				std::cout << "TMPBODY APRES SUPPRESSION BOUNDARY" << std::endl;
				std::cout << tmpBody << std::endl;
				_clientRef.setState(READING_MULTIPART_HEADER);
				std::cout << "CLIENT INFO" << std::endl;
				_clientRef.printClient();
			}

			if (_clientRef.getState() == READING_MULTIPART_HEADER)
			{
				std::cout << "ON VA LIRE LE HEADER" << std::endl;

				pos = tmpBody.find("\r\n\r\n");
				if (pos == std::string::npos)
					throw Request::ErrcodeException(BAD_REQUEST, *this);

				std::string header = tmpBody.substr(0, pos);
				pos = header.find("filename=");
				std::string filename = header.substr(pos + 9, 2);
				if (filename == "\"\"")
				{
					std::cout << " ON ENTRE DANS LE FILNAME VIDE" << std::endl;
					pos = tmpBody.find(_clientRef.getBoundary());
					tmpBody.erase(0, pos);
					_clientRef.setState(READING_BOUNDARY);
				}
				else
				{
					std::cout << " ON ENTRE DANS LE FILNAME PAS VIDE" << std::endl;
					pos = tmpBody.find("\r\n\r\n");
					generateUniqueFilename();
					std::cout << "NOUVEAU FICHIER GENERER = " << _clientRef.getFilename() << std::endl;
					tmpBody.erase(0, pos + 4);
					std::cout << "TMPBODY APRES SUPPRESSION HEADER" << std::endl;
					std::cout << tmpBody << std::endl;
					_clientRef.setState(READING_MULTIPART_DATA);
					std::cout << "CLIENT INFO" << std::endl;
					_clientRef.printClient();
				}
			}

			if (_clientRef.getState() == READING_MULTIPART_DATA)
			{
				std::cout << "ON VA LIRE LES DATA" << std::endl;

				if (!_clientRef.getPartialBuffer().empty())
				{
					std::cout << "ON ENTRE CAR IL Y A UN PARTIAL BUFFER" << std::endl;
					tmpBody = _clientRef.getPartialBuffer() + tmpBody;
				}
				pos = tmpBody.find(_clientRef.getBoundary());
				if (pos == std::string::npos)
				{
					std::cout << "ON TROUVE PAS LE BOUNDARY DANS READ DATA" << std::endl;
					_clientRef.setPartialBuffer(tmpBody.substr(tmpBody.size() > 100 ? tmpBody.size() - 100 : 0));
					_body = tmpBody.substr(0, tmpBody.size() > 100 ? tmpBody.size() - 100 : 0);
					cgiManager c(*this, _clientRef);
					c.execute();
				}
				else
				{
					std::cout << "ON TROUVE  LE BOUNDARY DANS READ DATA" << std::endl;
					std::cout << "TMPBODY AVANT DE LE METTRE DANS LE BODY" << std::endl;
					std::cout << tmpBody << std::endl;
					_body = tmpBody.substr(0, pos);
					std::cout << "BODY QU'ON ENVOIE AU CGI" << std::endl;
					std::cout << _body << std::endl;
					tmpBody.erase(0, pos);
					std::cout << "TMPBODY APRES SUPPRESSION DATA" << std::endl;
					std::cout << tmpBody << std::endl;
					_clientRef.setState(READING_BOUNDARY);
					std::cout << "CLIENT INFO" << std::endl;
					_clientRef.printClient();
					cgiManager c(*this, _clientRef);
					c.execute();
				}
			}
		}
		// std::cout << std::string(30, '-') << std::endl;
		// std::cout << " INFO CLIENT APRES = " << std::endl;
		// _clientRef.printClient();
		// std::cout << std::string(30, '-') << std::endl;
		// std::cout << std::string(30, '-') << std::endl;
		// std::cout << " BODY APRES LES 3 ETAPES = " << std::endl;
		// std::cout << tmpBody << std::endl;
		// std::cout << std::string(30, '-') << std::endl;

		if (_mime == "application/x-httpd-php" || _mime == "text/x-python")
			throw Request::CGIcalled();
	}
}
