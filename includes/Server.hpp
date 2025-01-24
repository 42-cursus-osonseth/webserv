#ifndef	SERVER_HPP
#define SERVER_HPP


#include <map>
#include <list>
#include <vector>
#include <string>
typedef struct s_location
{
}	t_location;

typedef struct s_cgi
{

}	t_cgi;

class Server
{
	private:
		std::vector<std::string>server_names;
		std::vector<std::string>listenPorts;
		size_t	clientMaxBodySize;
	public:
		Server();
		~Server();
		void	addPorts(const std::string &portStr);
		void	addServerNames(const std::string &serverName);
		void	setMaxBodySize(size_t value);
};

#endif