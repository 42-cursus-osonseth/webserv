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
    int _id;
    ssize_t _contentLength;
    ssize_t _bytesRead;
    bool _bodyFullyRead;
    std::string _body;
    std::string _filename;
    std::string _contentType;
    size_t _currentChunkRead;
    size_t _chunkSize;
    bool _isChunk;
    t_state _state;

public:
    client();
    client(int fd);
    ~client();

    int getId() const;
    const std::string &getBody() const;
    ssize_t getContentLenght() const;
    ssize_t getBytesRead() const;
    bool getBodyFullyRead() const;
    const std::string &getFilename() const;
    const std::string &getContentType() const;
    bool getIsChunk() const;
    size_t getCurrentChunkread() const;
    size_t getChunkSize() const;
    t_state GetState() const;
    void setBytesRead(ssize_t n);
    void setContentLength(ssize_t n);
    void setBobyFullyRead(bool b);
    void setFilename(std::string name);
    void setContentType(std::string type);
    void setIsChunck(bool b);
    void setChunkSize(size_t s);
    void setCurrentChunkRead(size_t s);
    void setState(t_state s);

    void printClient() const;
};
