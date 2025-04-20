#pragma once 

#include <iostream>
#include <sys/types.h>

class client
{
private:
    int _id;
    ssize_t _contentLength;
    ssize_t	_bytesRead;
    bool _bodyFullyRead;
    std::string _body;
    std::string _filename;
    std::string _contentType;

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
    void setBytesRead(ssize_t n);
    void setContentLength(ssize_t n);
    void setBobyFullyRead(bool b);
    void setFilename(std::string name);
    void setContentType(std::string type);

    void printClient() const;
   
};


