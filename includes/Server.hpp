#ifndef	SERVER_HPP
#define SERVER_HPP

#include "library.hpp"
#define DEFAULT_PORT 8080
#define DEFAULT_ADDR "localhost"
typedef struct s_location
{
	std::string root;
	std::string uri;
	std::string index;
	bool		dirListing;
	std::pair<int, std::string>			redir;
	std::list<std::string> methods;
	bool operator==(const s_location &location) const
	{
		return (location.uri == uri);
	}
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
		std::string hostAddress;
		std::list<t_location> locations;
		static std::list<Server> serversList;
		std::map<std::vector<int>, std::string> errorPages; 
	public:
		Server();
		~Server();
		static void	addServer(Server &server);
		static void	printServer();
		void	removeServer(Server &server);
		void	addPorts(const std::string &portStr);
		void	addServerNames(const std::string &serverName);
		void	addHostAddress(const std::string &hostAddress);
		void	setMaxBodySize(size_t value);
		void	setRoot(const std::string &root);
		void	setIndex(const std::string &index);
		void	addErrorPage(const std::pair<std::vector<int>, std::string>);
		void	setLocation(const t_location &location);
		void	addLocation(const t_location &location);
		std::list<Server>	findHost(Server	& server);
};

#endif