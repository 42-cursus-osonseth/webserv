#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "library.hpp"
#ifndef MAX_EVENTS
#define MAX_EVENTS 1024
#endif


#include "Server.hpp"
class webServer
{
	private:
		int epfd;
		int nfds;
		struct epoll_event ev, events[MAX_EVENTS];
		std::list<Server> serversList;
	public:
		webServer();
		webServer(const webServer &src);
		webServer &operator=(const webServer &rhs);
		~webServer();
		void handleSignals();
		void initEpoll();
		void setupServers();
		void closeWebServer();
		void start();

};

#endif 