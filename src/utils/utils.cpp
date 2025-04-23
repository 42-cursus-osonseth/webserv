#include <utils.hpp>
#include <sys/stat.h>
#include <stdio.h>
#include <sstream>

std::vector<std::string>	Utils::split(const std::string &s, const std::string &sep)
{
	size_t start = 0;
	size_t end;
	std::vector<std::string> tokens;

	while ((end = s.find(sep, start)) != std::string::npos) {
		tokens.push_back(s.substr(start, end - start));
		start = end + sep.length();
	}
	tokens.push_back(s.substr(start));
	return tokens;
}

std::string	Utils::getExtension(const std::string &s)
{
	size_t		len;

	if ((len = s.find_last_of(".")) != std::string::npos)
		return s.substr(len + 1);
	return "";
}

bool	Utils::pathIsDir(const std::string &path)
{
	struct stat	s;
	
	if (stat(path.c_str(), &s))
		perror("stat");
	return (!!(s.st_mode & S_IFDIR));
}

bool	Utils::fileExists(const std::string &path)
{
	struct stat	s;

	return (stat(path.c_str(), &s) == 0);
}

std::string	Utils::time_string(void)
{
	time_t timestamp = time(NULL);
	struct tm datetime = *localtime(&timestamp);
	std::string	resp = asctime(&datetime);

	resp.replace(resp.find("\n"), 1, "\r\n");
	return (resp);
}

std::string	Utils::itos(int n)
{
	std::stringstream	ss;

	ss << n;
	return ss.str();
}

std::string	Utils::cleanupPath(std::string path)
{
	std::string result;
	bool lastWasSlash = false;

	for (int i = 0; path[i]; i++) {
		char	ch = path[i];
		if (ch == '/') {
			if (!lastWasSlash)
				result += ch;
			lastWasSlash = true;
		} else {
			result += ch;
			lastWasSlash = false;
		}
	}
	return result;
}
size_t	Utils::htoi(const std::string &s)
{
	size_t	value;
	s.substr(0, s.find("\r\n"));
	
	std::istringstream iss(s);
	if (s.size() == 0)
		return (0);
	iss >> std::hex >> value;
	return value;
}
bool Utils::isValidChunkSize(const std::string& str)
{
    
    if (str.size() > 6)
		return false;
    for (size_t i = 0; i < str.size(); ++i) 
	{
        char c = str[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) 
            return false;
    }

    return true;
}
