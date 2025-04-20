#include <request.hpp>

void Request::readRemainingBody()
{
	_body = getRequest();
	_clientRef.setBytesRead(_body.size() + _clientRef.getBytesRead());
	_clientRef.printClient();
	debugPrintBodyReadable(10);
	if(_clientRef.getBytesRead() < _clientRef.getContentLenght())
		_clientRef.setBobyFullyRead(false);
	else if (_clientRef.getBytesRead() == _clientRef.getContentLenght())
		_clientRef.setBobyFullyRead(true);
	else
		std::cerr << "ERROR lecture superieur au content length" << std::endl;
}

void Request::postReq()
{

	getRessourcePath();
	_mime = get_mime(Utils::getExtension(_path));
	_clientRef.setBytesRead(_body.size() + _clientRef.getBytesRead());
	std::istringstream iss(_data["Content-Length"]);
	ssize_t len;
	iss >> len;
	 _clientRef.setContentLength(len);
	//--------------------------------------------------------------------------
	// for (std::map<std::string, std::string>::iterator it = _data.begin(); it != _data.end(); ++it)
	// 	std::cout << it->first << ": " << it->second << std::endl;
	// std::cout << std::string(30, '-') << std::endl;
	// std::size_t pos = _data["Content-Type"].find("boundary=");
	// std::string boundary = _data["Content-Type"].substr(pos + 9);
	// std::string boudaryEnd = boundary + "--";
	// std::cout << "BOUNDARY = " << boundary << std::endl;
	_clientRef.printClient();
	debugPrintBodyReadable(10);
	//--------------------------------------------------------------------------
	// std::cout << "BODY SYZE = " << _body.size() << std::endl;


	// std::cout << std::string(30, '-') << std::endl;
	// std::cout << "METHOD = " << _method << " PATH = " << _fullPath << std::endl;
	// _clientRef.printClient();
	// std::cout << std::string(30, '-') << std::endl;

	if(_clientRef.getBytesRead() < _clientRef.getContentLenght())
		_clientRef.setBobyFullyRead(false);
	else if (_clientRef.getBytesRead() == _clientRef.getContentLenght())
		_clientRef.setBobyFullyRead(true);
	else
		std::cerr << "ERROR lecture superieur au content length" << std::endl;


	getBody();
	if (_mime == "application/x-httpd-php")
		throw Request::CGIcalled();
}
void Request::debugPrintBodyReadable(int nbLines) {
	std::istringstream stream(_body);
	std::string line;
	int count = 0;
	size_t headerSize = 0;

	std::cout << "=== Début du body ===" << std::endl;

	// On tente d’afficher les premières lignes lisibles
	while (std::getline(stream, line) && count < nbLines) {
		// Retirer le \r si présent
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		// On ne print que si la ligne est lisible (ASCII)
		bool isBinary = false;
		for (size_t i = 0; i < line.size(); ++i) {
			if ((unsigned char)line[i] < 32 && line[i] != '\r' && line[i] != '\n' && line[i] != '\t') {
				isBinary = true;
				break;
			}
		}
		if (isBinary) break;

		std::cout << line << std::endl;
		headerSize += line.size() + 2; // +2 pour \r\n
		count++;

		if (line.empty()) break;
	}

	size_t remainingSize = _body.size() - headerSize;
	std::cout << "[" << remainingSize << " octets de données]" << std::endl;

	// On affiche le boundary de fin s’il est là
	std::string endBoundary = _body.substr(_body.size() > 80 ? _body.size() - 80 : 0);
	size_t pos = endBoundary.find("--");
	if (pos != std::string::npos)
		std::cout << endBoundary.substr(pos) << std::endl;

	std::cout << "=== Fin du body ===" << std::endl;
}
