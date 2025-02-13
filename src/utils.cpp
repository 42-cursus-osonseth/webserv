#include "../include/utils.hpp"
#include <sys/stat.h>

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
	std::string	ext = "";
	std::string	prev = s;
	size_t		len;

	while ((len = prev.find(".")) != std::string::npos) {
		ext = prev.substr(len + 1);
		prev = ext;
	}
	return ext;
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
