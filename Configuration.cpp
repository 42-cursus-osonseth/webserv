#include "includes/Configuration.hpp"
#include <functional>
int Configuration::nbServer = 0; 
std::ifstream Configuration::infile; 


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
    infile.get(c);
    word += c;
  }

  return word;
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

void	Configuration::parseBlock()
{
	int	nestedLevel = 1, lineNbr = 1;
	bool	isQuoted = false;
	std::string line;
	do
	{
		getline(infile, line);
		line = trim(line);
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
				break;
			}
			else if (line[i] == '}' && nestedLevel < 0)
			{
				throw BraceNotClosedException();
			}
		}
		lineNbr++;
	} while (infile.good() && !infile.eof());
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
					// std::cout << MAGENTA << "loop properly\n" << RESET;
			}
			catch(const std::exception& e)
			{
				std::cerr << SUPA_RED <<  e.what() << '\n';
			}
		}
		if (nbServer < 1)
			throw()
	}
	std::cout << "number of server = "<< nbServer << '\n';
	infile.close();
}

const char* Configuration::BraceNotClosedException::what() const throw() {return "Error: braces not close";}
const char* Configuration::NoserverFoundException::what() const throw() {return "Error: 0 server block found";}