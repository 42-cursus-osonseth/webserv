#pragma once

#include <iostream>
#include <sys/types.h>

typedef enum
{
    READING_CHUNK_SIZE,
    READING_CHUNK_DATA,
} t_state;

class client
{

private:
    int _fd;
    ssize_t _contentLength;
    ssize_t _bytesRead;
    size_t _currentChunkRead;
    size_t _chunkSize;
    bool _bodyFullyRead;
    bool _isChunk;
    bool _readCRLFfirst;
    t_state _state;

    std::string _body;
    std::string _method;
    std::string _filename;
    std::string _contentType;
    std::string _partialChunkSize;
    std::string _mime;
    std::string _path;
   

public:
    client();
    client(int fd);
    ~client();

    int getFd() const;
    const std::string &getBody() const;
    ssize_t getContentLenght() const;
    ssize_t getBytesRead() const;
    bool getBodyFullyRead() const;
    bool getReadCRLFfirst() const;
    const std::string &getFilename() const;
    const std::string &getContentType() const;
    bool getIsChunk() const;
    size_t getCurrentChunkread() const;
    size_t getChunkSize() const;
    t_state GetState() const;
    const std::string &getPartialChunkSize() const;
    const std::string &getMime() const;
    const std::string &getPath() const;
    const std::string &getMethod() const;
    

    void setBytesRead(ssize_t n);
    void setContentLength(ssize_t n);
    void setBobyFullyRead(bool b);
    void setFilename(std::string name);
    void setContentType(std::string type);
    void setIsChunck(bool b);
    void setChunkSize(size_t s);
    void setCurrentChunkRead(size_t s);
    void setState(t_state s);
    void setPartialChunkSize(std::string str);
    void setMime(std::string str);
    void setPath(std::string str);
    void setMethod(std::string str);
    void setReadCRLFfirst(bool b);

    void printClient() const;
};
