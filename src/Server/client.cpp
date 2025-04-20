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
const std::string &client::getFilename() const
{
    return _filename;
}
const std::string &client::getContentType() const
{
    return _contentType;
}
void client::setBytesRead(ssize_t n)
{
    _bytesRead = n;
}
void client::setContentLength(ssize_t n)
{
    _contentLength = n;
}
void client::setBobyFullyRead(bool b)
{
    _bodyFullyRead = b;
}
void client::setFilename(std::string name)
{
    _filename = name;
}
void client::setContentType(std::string type)
{
    _contentType = type;
}
void client::printClient() const
{
    std::cout << std::string(30,'-') << std::endl;
    std::cout << "ID = " << _id << std::endl;
    std::cout << "CONTENT LENGTH = " << _contentLength << std::endl;
    std::cout << "BYTES READ = " << _bytesRead << std::endl;
    std::cout << "BOOL FULL READ = " << _bodyFullyRead << std::endl;
    std::cout << "FILENAME = " << _filename << std::endl;
    std::cout << "CONTENT TYPE = " << _contentType << std::endl;
    std::cout << std::string(30,'-') << std::endl;
}