#ifndef CHECKFILES_HPP
#define CHECKFILES_HPP

#include "library.hpp"	
bool						isValidFormat(const std::string &filename);
bool						isValidFile(const std::string &filename);
bool						isValidDir(const std::string &path);
std::string					formatPath(const std::string &path);
#endif