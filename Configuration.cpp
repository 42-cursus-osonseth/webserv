#include "includes/Configuration.hpp"

int Configuration::nbServer = 0; 

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

void	Configuration::parseServerBlock(std::ifstream & o, const std::string line;)
void	Configuration::parseFile(const std::string &filename)
{
	std::string line;
	std::ifstream o(filename.c_str());
	if (!o.is_open())
		throw std::runtime_error("Error: Failed to open file:" + filename + '\n');
	if (o.good())
	{
		while (getline(o, line))
		{
			try
			{
				std::cout << line << '\n';
				if (trim(line) == "server")
				{
					Configuration::nbServer++;
				}
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
	}
	std::cout << "number of server = "<< nbServer << '\n';
	o.close();
}