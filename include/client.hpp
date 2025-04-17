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

public:

    client();
    client(int fd);
    ~client();

    int getId() const;
    const std::string &getBody() const;
    ssize_t getContentLenght() const;
    ssize_t getBytesRead() const;
    bool getBodyFullyRead() const;

    void printClient() const;
   
};


