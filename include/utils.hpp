#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <vector>

class Utils {
private:
	Utils();
	~Utils();
public:
	static std::vector<std::string>	split(const std::string	&s, const std::string &sep);
	static std::string				getExtension(const std::string &s);
	static bool						pathIsDir(const std::string &path);
	static bool						fileExists(const std::string &path);
	static std::string				time_string(void);
	static std::string				itos(int);
	static std::string				cleanupPath(std::string);
	static size_t					htoi(const std::string &);
	static bool						isValidChunkSize(const std::string& str);
	static void						replaceLitterals(std::string &str);
};

#endif
