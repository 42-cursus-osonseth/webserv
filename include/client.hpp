#pragma once 

#include <ctime>
#include <iostream>


class client
{
private:
    int _id;
    ssize_t _contentLength;
    ssize_t	_bytesRead;
    bool _bodyFullyRead;

public:

    client();
    client(int fd);
    ~client();

    int getId() const;
    ssize_t getContentLenght() const;
    ssize_t getBytesRead() const;
    bool getBodyFullyRead() const;
   
};


