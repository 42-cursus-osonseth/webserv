#include "client.hpp"

client::client()
{

}
client::client(int fd) : _id(fd), _contentLength(0), _bytesRead(0), _bodyFullyRead(true)
{

}
client::~client()
{

}
int client::getId() const
{
    return _id;
}

ssize_t client::getContentLenght() const
{
    return _contentLength;
}
ssize_t client::getBytesRead() const
{
    return _bytesRead;
}
bool client::getBodyFullyRead() const
{
    return _bodyFullyRead;
}
const std::string &client::getBody() const
{
    return _body;
}
void client::printClient() const
{
    std::cout << "ID = " << _id << std::endl;
    std::cout << "CONTENT LENGTH = " << _contentLength << std::endl;
    std::cout << "BYTES READ = " << _bytesRead << std::endl;
    std::cout << "BOOL FULL READ = " << _bodyFullyRead << std::endl;
}