#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <exception>
#include <stdexcept>

class Request;

class cgiManager
{
private:

    int _fd;
    std::string _body;
    std::string _method;
    
    std::string _path;
    std::string _response;
    std::string _contentLength;
    int _pid;
    int _bytesRead;
    char * _args[2];
    char * _env[5];
    int _sv_in[2];
    int _sv_out[2];
    char _buffer[1024];

    void initPostEnv(int length);
    void executePostRequest();
    void executeGetRequest();

public:
    cgiManager();
    cgiManager(Request &req);
    ~cgiManager();
    void execute();
};