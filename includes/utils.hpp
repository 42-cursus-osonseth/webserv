#ifndef	UTILS_HPP
#define UTILS_HPP

template < typename T > T convert(const std::string& str) {
        std::istringstream iss(str);
        T                  value;
        iss >> value;
        if (iss.fail() || !iss.eof()) {
                throw std::runtime_error("Error: impossible to convert.");
        }
        return value;
}

bool						isValidFormat(const std::string &filename);
bool						isValidFile(const std::string &filename);
bool                                            isEmptyLine(const std::string &line);
std::string					trim(const std::string& str);
bool						isStringDigit(const std::string &line);
bool						strIsAlpha(const std::string &str);
size_t						findFirstOf(const std::string &str, char c);
std::vector<std::string>	split(const std::string& str);


#endif