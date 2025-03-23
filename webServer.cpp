#include "webServer.hpp"

webServer::webServer(): epfd(-1), nfds(0), ev(), events(){}
webServer::~webServer(){}
webServer::webServer(const webServer &src)
{
	*this = src;
}
webServer &webServer::operator=(const webServer &rhs)
{
	if (this != &rhs)
	{
		this->epfd = rhs.epfd;
		this->nfds = rhs.nfds;
		this->ev = rhs.ev;
		this->events = rhs.events;
		this->serversList = rhs.serversList;
	}
	return *this;
}

void webServer::start()
{
	try
	{
		handleSignals();
		initEpoll();
		setupServers();
	}
	catch(const std::exception& e)
	{
		closeWebServer();
		std::cerr << e.what() << '\n';
	}
	
}