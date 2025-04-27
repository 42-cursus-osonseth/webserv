#ifndef	SERVER_HPP
#define SERVER_HPP

#include "library.hpp"
#include "errcodes.hpp"
#define DEFAULT_PORT 8080
#define DEFAULT_ADDR "localhost"
#include <sys/socket.h>
#include <arpa/inet.h>
typedef struct s_location
{
	std::string root;
	std::string uri;
	std::string index;
	bool		dirListing;
	std::pair<int, std::string>			redir;
	std::list<std::string> methods;
	bool		upload;
	std::string uploadPath;
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
		std::map<int, std::string> errorPages;
		int		sockfd;
		std::vector<int> sockfds;
		std::string		number_server;
		struct sockaddr_in   addr;
		public:
		//constructors
		Server();
		~Server();
		Server(const Server &src);
		Server &operator=(const Server &rhs);
		
		//sort locations
		void	sortLocations();

	
		void	acceptConnection(int epfd, struct epoll_event events);
		void	initSocket();
		void	closeSocket();
		static void	printServer();
		void	checkRequiredElements();
		void	printLocation(const t_location &location);
		void	removeServer(Server &server);
		std::list<Server>	findHost(Server	& server);
		int findNumberHost();
		
		//setters
		static void	addServer(Server &server);
		void	addPorts(const std::string &portStr);
		void	addServerNames(const std::string &serverName);
		void	addHostAddress(const std::string &hostAddress);
		void	setMaxBodySize(size_t value);
		void	setRoot(const std::string &root);
		void	setIndex(const std::string &index);
		void	addErrorPage(const std::pair<int, std::string>);
		void	setLocation(const t_location &location);
		void	addLocation(const t_location &location);
		
		
		//getters
		std::string	get_errcode_string(t_errcodes e);
		static Server* getInstance(const std::string& host, int port);
		const int &getSockfd() const;
		const std::vector<int>& getSockfds() const;
		std::string	getHostAddress();
		const std::list<t_location>& getLocations() const;
		const std::map<int, std::string>& getErrorPages() const;
		const std::vector<std::string>& getServerNames() const;
		const std::vector<std::string>& getListenPorts() const;
		size_t getClientMaxBodySize() const;
		const std::string& getIndex() const;
		const std::string& getRoot() const;
		const std::string& getHostAddress() const;
		static std::list<Server>& getServersList();
};

#endif