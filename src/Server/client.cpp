#include "client.hpp"

client::client()
{
}
client::client(int fd) : _fd(fd), _contentLength(0), _bytesRead(0), _currentChunkRead(0), _chunkSize(0), _bodyFullyRead(true), _isChunk(false), _readCRLFfirst(false), _state(READING_CHUNK_SIZE)
{
}
client::~client()
{
}
int client::getFd() const
{
    return _fd;
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
bool client::getReadCRLFfirst() const
{
    return _readCRLFfirst;
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
bool client::getIsChunk() const
{
    return _isChunk;
}
size_t client::getCurrentChunkread() const
{
    return _currentChunkRead;
}
size_t client::getChunkSize() const
{
    return _chunkSize;
}
t_state client::getState() const
{
    return _state;
}
const std::string &client::getPartialChunkSize() const
{
    return _partialChunkSize;
}
const std::string &client::getMime() const
{
    return _mime;
}
const std::string &client::getPath() const
{
    return _path;
}
const std::string &client::getMethod() const
{
    return _method;
}
const std::string &client::getBoundary() const
{
    return _boundary;
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
void client::setIsChunck(bool b)
{
    _isChunk = b;
}
void client::setChunkSize(size_t s)
{
    _chunkSize = s;
}
void client::setCurrentChunkRead(size_t s)
{
    _currentChunkRead = s;
}
void client::setState(t_state s)
{
    _state = s;
}
void client::setPartialChunkSize(std::string str)
{
    _partialChunkSize = str;
}
void client::setMime(std::string str)
{
    _mime = str;
}
void client::setPath(std::string str)
{
    _path = str;
}
void client::setMethod(std::string str)
{
    _method = str;
}
void client::setReadCRLFfirst(bool b)
{
    _readCRLFfirst = b;
}
void client::setBoundary(std::string str)
{
    _boundary = str;
}
void client::printClient() const
{
    std::cout << std::string(30, '-') << std::endl;
    std::cout << "ID = " << _fd << std::endl;
    std::cout << "CONTENT LENGTH = " << _contentLength << std::endl;
    std::cout << "BYTES READ = " << _bytesRead << std::endl;
    std::cout << "BOOL FULL READ = " << _bodyFullyRead << std::endl;
    std::cout << "FILENAME = " << _filename << std::endl;
    std::cout << "CONTENT TYPE = " << _contentType << std::endl;
    std::cout << "IS CHUNK = " << _isChunk << std::endl;
    std::cout << "BOUNDARY = " << _boundary << std::endl;

    std::cout << std::string(30, '-') << std::endl;
}