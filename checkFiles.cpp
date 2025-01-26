#include "includes/Configuration.hpp"
#include <sys/stat.h> 
#include <unistd.h>


bool	isValidFormat(const std::string &filename) {
	struct stat file_stat;
	if (stat(filename.c_str(), &file_stat))
		return false;
	// if (file_stat.st_mode & S_IFREG)
	// 	return false;
	if (file_stat.st_mode & S_IFDIR)
		return false;
	return true;
}

bool	isValidFile(const std::string &filename)
{
	// if (filename[0] == '/')
		// filename = filename.substr(1);
	if (!isValidFormat(filename) || access(filename.c_str(), R_OK))
		return false;
	return true;
}