#include "client.hpp"

client::client()
{

}
client::client(int fd) : _id(fd), _contentLength(0), _bytesRead(0)
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