#include "client.hpp"

client::client()
{
}

client::~client()
{
}

client::client(int fd) : _id(fd)
{
    _last_request = std::time(NULL);
}
void client::update_time()
{
  std::time(&_last_request);
}
int client::get_id() const
{
    return _id;
}
std::string client::get_cookies() const
{
    return _cookies;
}
std::time_t client::get_last_request_time() const
{
    return _last_request;
}

void client::print_client() const
{
    std::cout << "ID : " << _id << std::endl;
    std::cout << "Last request : " << _last_request << std::endl;
    std::cout << "Cookies : " << _cookies << std::endl;
}