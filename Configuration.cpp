#include "includes/Configuration.hpp"
#include <functional>
int Configuration::nbServer = 0; 
std::ifstream Configuration::infile; 



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
	int	nestedLevel = 0;
	bool	isQuoted = false, isEscaped = false;
	std::string line;
	do
	{
		getline(infile, line);
		line = trim(line);
		for (size_t i ; i < line.size();i++)
		{
			if (line[i] == '"')
				isQuoted = !isQuoted;
			else if (line[i] == '\\')
				isEscaped = true;
			else if (line[i] == '#')
				break;
			else if (line[i] == '{')
				nestedLevel++;
			else if (line[i] && --nestedLevel < 0)
				return;
		}
		if (nestedLevel != 0 || nestedLevel == 0)
	} while (getline);
	
}

void	handleEvents()
{

}
void	handleHttp()
{

}

bool	Configuration::handleServer()
{
	if (readNextWord(infile) != "{")
		return false;
	parseBlock();
}

bool	Configuration::handleToken(const std::string &token)
{

	// std::map<std::string, void()> blockhandle = {
	// 	{"events", handleEvents},
	// 	{"http", handleHttp},
	// 	{"server", handleServer}
	// };
	std::string line;
	if (token == "events")
		handleEvents();
	else if (token == "http")
		handleHttp();
	else if (token == "server")
		handleServer();
	else if (token == "#")
		getline(infile, line);
	else
		return false;
	// if (readNextWord(infile) != "{")
	// 	return	false;
	// while (readNextWord(infile) != "}")
	// return false;
}

std::string readNextWord(std::ifstream& infile) {
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

std::string trim(const std::string& str) 
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
        return "";

    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

Configuration::Configuration(){}

Configuration::~Configuration(){}


void	Configuration::parseFile(const std::string &filename)
{
	std::string line;
	infile.open(filename.c_str());
	if (!isValidFile(filename) || !infile.is_open())
		throw std::runtime_error("Error: Failed to open file: " + filename + '\n');

	if (infile.good())
	{
		while (getline(infile, line))
		{
			try
			{
					if (!handleToken(readNextWord(infile))){
						throw std::runtime_error("Error: parsing file: " + line + '\n');
					}
			}
			catch(const std::exception& e)
			{
				std::cerr << SUPA_RED <<  e.what() << '\n';
			}
		}
	}
	std::cout << "number of server = "<< nbServer << '\n';
	infile.close();
}