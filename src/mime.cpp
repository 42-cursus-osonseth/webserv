#include <mime.hpp>

std::string	get_mime(const std::string &key)
{
	static	std::map<std::string, std::string>	strings;

	if (strings.size() == 0) {
		strings["html"] = "text/html";
		strings["htm"] = "text/html";
		strings["css"] = "text/css";
		strings["ico"] = "image/x-icon";
		strings["avi"] = "video/x-msvideo";
		strings["bmp"] = "image/bmp";
		strings["doc"] = "application/msword";
		strings["gif"] = "image/gif";
		strings["gz"] = "application/x-gzip";
		strings["jpg"] = "image/jpg";
		strings["jpeg"] = "image/jpeg";
		strings["png"] = "image/png";
		strings["txt"] = "text/plain";
		strings["mp3"] = "audio/mp3";
		strings["pdf"] = "application/pdf";
		strings["js"] = "application/javascript";
		strings["py"] = "text/x-python";
		strings["php"] = "application/x-httpd-php";
	}
	return (strings.at(key));
}
