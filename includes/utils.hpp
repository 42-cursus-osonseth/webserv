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

bool	isValidFormat(const std::string &filename);
bool	isValidFile(const std::string &filename);

#endif