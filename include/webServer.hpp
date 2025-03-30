#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "library.hpp"
#ifndef MAX_EVENTS
#define MAX_EVENTS 1024
#endif

#include <sys/epoll.h>
#include "Server.hpp"
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
	public:
		webServer();
		webServer(const webServer &src);
		webServer (std::list<Server> serversList);
		webServer &operator=(const webServer &rhs);
		~webServer();
		void initEpoll();
		void setupServers();
		void closeWebServer();
		void acceptConnection(Server const &server);
		void start();

};

#endif 