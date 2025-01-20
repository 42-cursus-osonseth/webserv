#include "includes/Configuration.hpp"
#include <functional>
int Configuration::nbServer = 0; 
std::ifstream Configuration::infile; 

std::vector<std::string> split(const std::string& str) {
  std::vector<std::string> tokens;
  std::istringstream iss(str);
  std::string token;
  while (iss >> token) 
    tokens.push_back(token);
  return tokens;
}

std::string trim(const std::string& str) 
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
        return "";

    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
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

void	Configuration::parseLocation(const std::string &line)
{
	return;
}

void	Configuration::parseCgi(const std::string &line)
{
	return;
}

void	Configuration::parsePorts(const std::string &line, Server &server)
{
	std::vector<std::string> serverPorts = split(skipWord(line));
	if (serverPorts.size() == 0)
	{
		
	}
	for (std::vector<std::string>::const_iterator it = serverPorts.begin();it != serverPorts.end();it++)
	{

	}
}
// void	checkSemiColon(const std::string &line)
// {
// 	if (line[line.size() - 1] != ';')
// 		throw()
// }


void	fillHost(Server &host)
{
		
}

void	Configuration::chooseDirectives(const std::string &line)
{
	if (line == "listen")
		parsePorts();
	else if (line == "server_name")
		parseServerName();
	else if (line == "error_page")
		parseErrorPage();
	else if (line == "client_max_body_size")
		parseMaxClients();
}

void	Configuration::parseBlock()
{
	Server	host;
	int	nestedLevel = 1, lineNbr = 1;
	bool	isQuoted = false, blockFound;
	std::string line;
	do
	{
		getline(infile, line);
		line = trim(line);
		if (getFirstWord(line) == "location")
			parseLocation(line);
		else if (getFirstWord(line) == "cgi")
			parseCgi(line);
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
			else if (i+1 == line.size() && line[i] != ';')
				throw MissingSemicolonException();
		}
		chooseDirectives(line);
		lineNbr++;
	} while (infile.good() && !infile.eof() && !blockFound);
	if (nestedLevel > 0)
		throw BraceNotClosedException();
	fillHost(host);
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
				std::cerr << SUPA_RED <<  e.what() << '\n';
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