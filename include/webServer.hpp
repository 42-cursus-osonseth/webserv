#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "library.hpp"
#ifndef MAX_EVENTS
#define MAX_EVENTS 1024
#endif

#include <sys/epoll.h>
#include "Server.hpp"
#include "client.hpp"

#define EDGE_TRIGGERED 0
#define LEVEL_TRIGGERED 1

class webServer
{
	private:
		int epfd;
		int nfds;
		struct epoll_event ev;
		struct epoll_event events[MAX_EVENTS];
		std::list<Server> serversList;
		std::vector<int> client_fds;
		std::vector<int> server_fds;
		std::map<int, client> clients;
	public:
		webServer();
		webServer(const webServer &src);
		webServer (std::list<Server> serversList);
		webServer &operator=(const webServer &rhs);
		~webServer();
		bool isServer(int fd);
		bool isClient(int fd);
		void initEpoll();
		void setupServers();
		void closeWebServer();
		void acceptConnection(int server_fd);
		void start();
		void setSocketMode(int client_fd, int mode);

};

#endif 