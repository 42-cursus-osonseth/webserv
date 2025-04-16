#include "cgiManager.hpp"
#include "request.hpp"
#include <stdio.h>
cgiManager::cgiManager()
{
}

cgiManager::~cgiManager()
{
}
void cgiManager::initPostEnv(int length)
{
    std::ostringstream oss;
    oss << length;
    std::string lenStr = oss.str();
    _contentLength = "CONTENT_LENGTH=" + lenStr;
    _env[0] = (char *)"REQUEST_METHOD=POST";
    _env[1] = (char *)"CONTENT_TYPE=application/x-www-form-urlencoded";
    _env[2] = (char *)_contentLength.c_str();
    _env[3] = NULL;
}

cgiManager::cgiManager(Request &req) : _fd(req.getFd()), _body(req.getterBody()), _method(req.getMethod())
{

    _sv_in[0] = -1;
    _sv_in[1] = -1;
    _sv_out[0] = -1;
    _sv_out[1] = -1;
    _args[0] = const_cast<char *>(req.getPath().c_str());
    _args[1] = NULL;
    if (_method == "POST")
        initPostEnv(_body.size());
    else
        _env[0] = NULL;
    memset(_buffer, 0, sizeof(_buffer));
}

void cgiManager::execute()
{
    if (_method == "POST")
    {
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, _sv_in) == -1)
            throw std::runtime_error("socketpair failed");
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, _sv_out) == -1)
            throw std::runtime_error("socketpair failed");
        if ((_pid = fork()) == -1)
            throw std::runtime_error("fork failed");
        if (_pid == 0)
        {
            close(_sv_in[1]);
            close(_sv_out[0]);
            dup2(_sv_in[0], STDIN_FILENO);
            dup2(_sv_out[1], STDOUT_FILENO);
            close(_sv_in[0]);
            close(_sv_out[1]);
            if (execve(_args[0], _args, _env) == -1)
                throw std::runtime_error("execve failed");
        }
        else
        {
            close(_sv_in[0]);
            close(_sv_out[1]);
            write(_sv_in[1], _body.c_str(), _body.size());
            close(_sv_in[1]);
            while ((_bytesRead = read(_sv_out[0], _buffer, sizeof(_buffer) - 1)) > 0)
                _response.append(_buffer, _bytesRead);
            close(_sv_out[0]);
        }
        send(_fd, _response.c_str(), _response.size(), 0);
    }
    else
    {
        if (socketpair(AF_UNIX, SOCK_STREAM, 0, _sv_in) == -1)
            throw std::runtime_error("socketpair failed");

        if ((_pid = fork()) == -1)
            throw std::runtime_error("fork failed");

        if (_pid == 0)
        {
            close(_sv_in[1]);
            dup2(_sv_in[0], STDOUT_FILENO);

            close(_sv_in[0]);

            if (execve(_args[0], _args, _env) == -1)
                throw std::runtime_error("execve failed");
        }
        else
        {
            close(_sv_in[0]);
            while ((_bytesRead = read(_sv_in[1], _buffer, sizeof(_buffer) - 1)) > 0)
                _response.append(_buffer, _bytesRead);
            close(_sv_in[1]);
            std::cout << " ----- REPONSE --------" << std::endl;
            std::cout << std::string(30,'-') << std::endl;
            std::cout << _response << std::endl;
            std::cout << std::string(30,'-') << std::endl;
            send(_fd, _response.c_str(), _response.size(), 0);
        }
    }
}

