#include "includes/Configuration.hpp"
#include <functional>
int Configuration::nbServer = 0; 
std::ifstream Configuration::infile;
bool Configuration::locationFlag = false;
bool Configuration::cgiFlag = false;
int	 Configuration::lineNbr = 2;

std::vector<std::string> split(const std::string& str) {
  std::vector<std::string> tokens;
  std::istringstream iss(str);
  std::string token;
  while (iss >> token) 
    tokens.push_back(token);
  return tokens;
}

size_t findFirstOf(const std::string &str, char c)
{
	for (size_t i = 0;i < str.size();i++)
		if (str[i] == 'c')
			return i;
	(void)c;
	return std::string::npos;
}

bool	strIsAlpha(const std::string &str)
{
	for (size_t i =0; i < str.size();i++)
		if (!isalpha(str[i]))
			return false;
	return true;
}

bool	isStringDigit(const std::string &line)
{
	for (size_t i = 0; i < line.size();i++)
		if (!std::isdigit(line[i]))
			return (false);
	return (true);
}

std::string trim(const std::string& str) 
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
        return "";

    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

void	skipComments(std::istream &istream)
{
	char	c;

	while (istream.peek() != EOF)
	{
		c = istream.get();
		if (c == '\n')
		{
			return;
		}
	}
}

bool	isValidDns(const std::string &str)
{
	std::vector<std::string> args;
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, '.'))
		args.push_back(token);
	if (args.size() < 2)
		return false;
	else if (args.size() == 3)
	{
		if (args[0] != "www")
			return (false);
	}
	return true;
}

bool	isIpAddress(const std::string &line)
{	
	size_t	sep = line.find(':');
	if (!line[sep])
		return false;
	std::string ipStr = line.substr(0, sep);
	std::string port = line.substr(sep + 1, line.size());
	std::vector<std::string> octets;
	std::istringstream iss(ipStr);
	std::string tmp;
	while (std::getline(iss, tmp, '.'))
	{
		if (!isStringDigit(tmp) || (std::atoi(tmp.c_str()) < 0 || std::atoi(tmp.c_str()) > 255))
			return (false);
		octets.push_back(tmp);
	}
	if (octets.size() != 4)
		return (false);
	if (!isStringDigit(port) || (std::atoi(port.c_str()) < 0 || std::atoi(port.c_str()) > 65535))
		return (false);	
	return (true);
}


std::string readNextWord(std::ifstream& infile)
{
  std::string word;
  char c;

  while (infile && isspace(infile.peek())) {
    infile.get(c); 
  }

  while (infile && !isspace(infile.peek())) {
	if (infile.peek() == '#')
		skipComments(infile);
    infile.get(c);
    word += c;
  }

  return word;
}

std::string skipWord(const std::string &line)
{
	std::istringstream iss(line);
	std::string wordRemove;
	iss >> wordRemove;
	std::stringstream lineWithoutFirstWord;
	lineWithoutFirstWord << iss.rdbuf();
	return lineWithoutFirstWord.str();
}


std::string getFirstWord(const std::string& line)
{
    std::string::size_type start = 0;
    while (start < line.length() && std::isspace(line[start]))
        ++start;
    std::string::size_type end = start;
    while (end < line.length() && !std::isspace(line[end]))
        ++end;
    return line.substr(start, end - start);
}

// void	Configuration::parseLocation(const std::string &line)
// {
// 	return;
// }

// void	Configuration::parseCgi(const std::string &line)
// {
// 	return;
// }

void	Configuration::parseRoot(const std::string &line, Server &server)
{
	std::string root = trim(skipWord(line));
	if (root.empty())
		throw MissingRootException();
	server.setRoot(root);
}

void	Configuration::parseIndex(const std::string &line, Server &server)
{
	std::string index = trim(skipWord(line));
	if (index.empty())
		throw MissingIndexException();
	server.setIndex(index);
}

void	Configuration::parsePorts(const std::string &line, Server &server)
{
	std::vector<std::string> serverPorts = split(skipWord(line));
	if (serverPorts.size() == 0)
		throw EmptyPortsException();
	for (std::vector<std::string>::const_iterator it = serverPorts.begin();it != serverPorts.end();it++)
	{
		if (!isStringDigit(*it) && *it != "default_server" && isIpAddress(*it))
			throw InvalidPortsException();
		server.addPorts(*it);
	}
}


void	Configuration::parseServerName(const std::string &line, Server &server)
{
	std::vector<std::string>serversName = split(skipWord(line));
	if (serversName.size() == 0)
		throw EmptyServerNameException();
	for (std::vector<std::string>::const_iterator it = serversName.begin(); it != serversName.end();it++)
	{
		if (findFirstOf(*it, '.') != std::string::npos)
			if (!isValidDns(*it))
				throw InvalidDnsException();
		server.addServerNames(*it);
	}
}

void	Configuration::parseErrorPage(const std::string &line, Server &server)
{
	std::vector<std::string>args = split(skipWord(line));
	if (args.size() == 0)
		throw EmptyPageErrorException();
	for (std::vector<std::string>::iterator it = args.begin();it != args.end() - 1;it++)
	{
		if (!isStringDigit(*it))
		{
			throw InvalidPageErrorException();
		}
	}
	if (args.back()[0] == '/')
		args.back() = args.back().substr(1);
	if (!isValidFile(args.back()))
			throw InvalidPageErrorException();
	std::vector<int>v;
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end()-1;it++)
		v.push_back(convert<int>(*it));
	server.addErrorPage(std::make_pair(v, args.back()));
}

void	Configuration::parseMaxClients(const std::string &line, Server &server)
{
	size_t value = 1;
	std::string arg = trim(skipWord(line));
	for (size_t i = 0; arg.size()-2;i++)
	{
		if (!isdigit(arg[i]))
			throw InvalidMaxBodySizeException();
		value *= 10 * (arg[i] - '0');
	}
	switch (toupper(arg[arg.size()-2]))
	{
		case 'K':
			value /= 1024;
			break;
		case 'M':
			break;
		case 'G':
			value *= 1024;
			break;
		default:
			throw InvalidMaxBodySizeException();
			break;
	}
	server.setMaxBodySize(value);
}


bool	Configuration::chooseDirectives(const std::string &lineWithSemicolon, Server &server)
{
	std::string line = lineWithSemicolon;
	if (line.empty() || line.size() < 2)
		return (false);
	if (line[line.size()-1] != ';')
		return true;
	line.resize(line.length()-1);
	std::string lineSplitted = split(line)[0];
	if (lineSplitted == "listen")
		return (parsePorts(line, server), true);
	else if (lineSplitted == "server_name")
		return (parseServerName(line, server), true);
	else if (lineSplitted == "error_page")
		return (parseErrorPage(line, server), true);
	else if (lineSplitted == "client_max_body_size")
		return (parseMaxClients(line, server), true);
	else if (lineSplitted == "root")
		return (parseRoot(line,server), true);
	else if (lineSplitted == "index")
		return (parseIndex(line,server), true);
	return (false);
}

bool	Configuration::chooseLocationDirectives(const std::string &line, t_location &location)
{
	(void)line;
	std::cout << "uri location: "<<location.uri << '\n';
	// if (line == "{")
		// return (false);
	// if (line == "root")
		// return(parseRoot(line,location), true);
	return (false);
// 	else if (line == "allow_methods")
// 		return (parseMethods(line,server),true);
// 	else if (line == "redirection")
// 		return (parseRedirection(line,server),true);
// 	else if (line == "directory_listing")
// 		return (parseDirListing(line,server),true);
// 	else if (line == "directory_file")
// 		return (parseDirFile(line,server),true);
// 	return (false);
}

void	Configuration::parseLocation(const std::string &line, Server &server)
{
	t_location	location;
	std::vector<std::string> lineSplited = split(skipWord(line));
	if (lineSplited.empty())
		throw	LocationArgsException("Error: missing uri for location");
	else if (lineSplited.size() > 2)
		throw	LocationArgsException("Error: Too more arguments for location");
	else if (lineSplited.size() == 2){
		if (trim(lineSplited[1]) != "{")
			throw BraceNotClosedException();
	}
	location.uri  = lineSplited[0];
	locationFlag = true;
	server.setLocation(location);
}

// void	Configuration::parseCgi()
// {

// }

void	Configuration::parseBlock()
{
	Server	host;
	int	nestedLevel = 1;
	bool	isQuoted = false, blockFound=false;
	std::string line;
	do
	{
		getline(infile, line);
		line = trim(line);
		if (getFirstWord(line) == "location" || getFirstWord(line) == "cgi")
		{
			parseLocation(line, host);
			lineNbr++;
			continue;
		}
		for (size_t i = 0; i < line.size();i++)
		{		
			if (line[i] == '"')
				isQuoted = !isQuoted;
			else if (line[i] == '#' && !isQuoted)
				break;
			else if (line[i] == '{')
				nestedLevel++;
			else if (line[i] == '}' && --nestedLevel == 0)
			{
				std::cout << "block found line: "<< lineNbr++ << '\n';
				blockFound = true;
				break;
			}
			else if (line[i] == '}' && nestedLevel < 0)
			{
				throw BraceNotClosedException();
			}
			else if (line[i] == '}')
			{
				if (locationFlag)
					locationFlag = false;
			}
		}
		if (locationFlag){
			if (chooseLocationDirectives(line, host.getLocation()) && line[line.size()-1] != ';')
				throw MissingSemicolonException();
		}
		else if (chooseDirectives(line, host) && line[line.size()-1] != ';')
				throw MissingSemicolonException();
		if (lineNbr == 5){
				std::cerr << line << " [" << lineNbr << "]\n";
				std::cerr << locationFlag;
				exit(1);
		}
		lineNbr++;
	} while (infile.good() && !infile.eof() && !blockFound);
	if (nestedLevel > 0)
		throw BraceNotClosedException();
}

bool	Configuration::handleServer()
{
	if (readNextWord(infile) != "{")
		return false;
	parseBlock();
	nbServer++;
	return true;
}

bool	Configuration::handleToken(const std::string &token)
{
	std::string line;
	if (token == "server")
		return handleServer();
	else if (token == "#")
		getline(infile, line);
	else
		return false;
	return (true);
}

Configuration::Configuration(){}

Configuration::~Configuration(){}

void	Configuration::parseFile(const std::string &filename)
{
	std::string line;
	infile.open(filename.c_str());
	if (!isValidFile(filename) || !infile.is_open())
		throw std::runtime_error("Error: Failed to open file: " + filename + '\n');
	int i = 1;
	if (infile.good())
	{
		while (infile.good())
		{
			try
			{
					std::cout << "ligne " << i << '\n';
					if (!handleToken(readNextWord(infile))){
						throw std::runtime_error("Error: parsing file: " + line + '\n');
					}
					std::cout << MAGENTA << "loop properly\n" << RESET;
			}
			catch(const std::exception& e)
			{
				std::cerr << SUPA_RED <<  e.what() << "\n\n" << RESET;
				return ;
			}
		}
		if (nbServer < 1)
			throw NoserverFoundException();
	}
	std::cout << "number of server = "<< nbServer << '\n';
	infile.close();
	(void)nbPort;
}

const char* Configuration::BraceNotClosedException::what() const throw() {return "Error: braces not close";}
const char* Configuration::NoserverFoundException::what() const throw() {return "Error: 0 server block found";}
const char* Configuration::MissingSemicolonException::what() const throw() {return "Error: Missing semicolon at the end of the line";}
const char* Configuration::EmptyPortsException::what() const throw() {return "Error: empty ports listen";}
const char* Configuration::InvalidPortsException::what() const throw() {return "Error: ports syntax invalid";}
const char* Configuration::EmptyServerNameException::what() const throw() {return "Error: empty server name";}
const char* Configuration::InvalidDnsException::what() const throw() {return "Error: Invalid dns syntax";}
const char* Configuration::EmptyPageErrorException::what() const throw() {return "Error: Missing page error";}
const char* Configuration::InvalidPageErrorException::what() const throw() {return "Error: Invalid page";}
const char* Configuration::InvalidMaxBodySizeException::what() const throw() {return "Error: Invalid Max body Size";}
const char* Configuration::MissingRootException::what() const throw() {return "Error: Missing root";}
const char* Configuration::MissingIndexException::what() const throw() {return "Error: Missing index";}
