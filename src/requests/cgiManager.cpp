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
            {
                std::cerr << "execve failed: " << strerror(errno) << std::endl;
                exit(1);
            }
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
        std::cout << std::string(20, '-') << std::endl;
        std::cout << "response = " << std::endl;
        std::cout << _response << std::endl;
        std::cout << std::string(20, '-') << std::endl;
        send(_fd, _response.c_str(), _response.size(), 0);
    }
}

// std::cout << " path dans class cgi = " << _path << std::endl;
// std::cout << " path dans args[0] = " << _args[0] << std::endl;
// if (socketpair(AF_UNIX, SOCK_STREAM, 0, _pipefd) == -1)
//     throw std::runtime_error("socketpair failed");
// if ((_pid = fork()) == -1)
//     throw std::runtime_error("fork failed");
// if (_pid == 0)
// {
//     std::cerr << "Avant execve" << std::endl; // Vérifie si on arrive ici
// dup2(_pipefd[0], STDIN_FILENO);
// dup2(_pipefd[0], STDOUT_FILENO);
//     close(_pipefd[1]);
//     close(_pipefd[0]);
//     if (execve(_args[0], _args, _env) == -1)
//     {
//         std::cerr << "execve failed: " << strerror(errno) << std::endl;
//         exit(1);
//     }
//     std::cerr << "Après execve" << std::endl; // Ce code ne doit pas être exécuté si execve réussit

// }
// else
// {
//     std::cout << std::string(20, '-') << std::endl;
//     std::cout << "METHOD = " << _method << std::endl;
//     std::cout << std::string(20, '-') << std::endl;
//     if (_method == "POST")
//     {
//         close(_pipefd[0]);
//         int read_fd = dup(_pipefd[1]);
//         std::cerr << "on rentre dans le if method == POST" << std::endl; // Vérifie si on arrive ici
//         write(_pipefd[1], _body.c_str(), _body.size());
//         close(_pipefd[1]);
//         std::cerr << "donne envoye dans le parent" << std::endl; // Vérifie si on arrive ici
//         std::cerr << "on a ecrit" << std::endl; // Vérifie si on arrive ici
//         while ((_bytesRead = read(read_fd, _buffer, sizeof(_buffer) - 1)) > 0)
//             _response.append(_buffer, _bytesRead);
//         close(read_fd);
//         std::cerr << "on a lu " << std::endl; // Vérifie si on arrive ici

//     }
//     else
//     {
//         close(_pipefd[1]);
//         while ((_bytesRead = read(_pipefd[0], _buffer, sizeof(_buffer) - 1)) > 0)
//             _response.append(_buffer, _bytesRead);
//         close(_pipefd[0]);

//     }
//     std::cout << std::string(20, '-') << std::endl;
//     std::cout << "response = " << _response << std::endl;
//     std::cout << std::string(20, '-') << std::endl;
//     waitpid(_pid, NULL, 0);
//     send(_fd, _response.c_str(), _response.size(), 0);
// }