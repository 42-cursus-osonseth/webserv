#ifndef	SERVER_HPP
#define SERVER_HPP


#include <map>
#include <list>
#include <vector>
#include <string>
#define DEFAULT_PORT 8080
#define DEFAULT_ADDR "localhost"
typedef struct s_location
{
	std::string root;
	std::string uri;
	int			redir;
	bool		dirListing;
	std::list<std::string> methods;
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
		std::string	index;
		std::string	root;
		std::map<std::vector<int>, std::string> errorPages; 
	public:
		Server();
		~Server();
		void	addPorts(const std::string &portStr);
		void	addServerNames(const std::string &serverName);
		void	setMaxBodySize(size_t value);
		void	setRoot(const std::string &root);
		void	setIndex(const std::string &index);
		void	addErrorPage(const std::pair<std::vector<int>, std::string>);
};

#endif