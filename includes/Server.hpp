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
		std::list<std::string> server_names;
	public:
		Server();
		~Server();
};

#endif