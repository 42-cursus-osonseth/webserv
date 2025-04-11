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
    // std::cout << "BODY = " << req.getterBody() << std::endl;
    // std::cout << "PATH = " << req.getPath() << std::endl;
    // std::cout << "METHOD = " <<req.getMethod() << std::endl;
    // std::cout << " test =  "<< std::hex << (int)req.getterBody()[23] << std::endl;
    _pipefd[0] = -1;
    _pipefd[1] = -1;
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
    // std::cout << " path dans class cgi = " << _path << std::endl;
    // std::cout << " path dans args[0] = " << _args[0] << std::endl;
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, _pipefd) == -1)
        throw std::runtime_error("socketpair failed");
    if ((_pid = fork()) == -1)
        throw std::runtime_error("fork failed");
    if (_pid == 0)
    {      
        std::cerr << "Avant execve" << std::endl; // Vérifie si on arrive ici
        dup2(_pipefd[0], STDIN_FILENO);
        dup2(_pipefd[1], STDOUT_FILENO);
        close(_pipefd[1]);
        close (_pipefd[0]);
        if (execve(_args[0], _args, _env) == -1)
        {
            std::cerr << "execve failed: " << strerror(errno) << std::endl;
            exit(1);
        }
        std::cerr << "Après execve" << std::endl; // Ce code ne doit pas être exécuté si execve réussit
       
    }
    else
    {
        std::cout << std::string(20, '-') << std::endl;
        std::cout << "METHOD = " << _method << std::endl;
        std::cout << std::string(20, '-') << std::endl;
        if (_method == "POST")
        {
            close(_pipefd[0]);
    
            std::cerr << "on rentre dans le if method == POST" << std::endl; // Vérifie si on arrive ici
            write(_pipefd[1], _body.c_str(), _body.size());
            std::cerr << "donne envoye dans le parent" << std::endl; // Vérifie si on arrive ici
            std::cerr << "on a ecrit" << std::endl; // Vérifie si on arrive ici
            while ((_bytesRead = read(_pipefd[0], _buffer, sizeof(_buffer) - 1)) > 0)
                _response.append(_buffer, _bytesRead);
            std::cerr << "on a lu " << std::endl; // Vérifie si on arrive ici
            close(_pipefd[1]);
        
        }
        else
        {
            close(_pipefd[1]);
            while ((_bytesRead = read(_pipefd[0], _buffer, sizeof(_buffer) - 1)) > 0)
                _response.append(_buffer, _bytesRead);
            close(_pipefd[0]);
    
        }
        std::cout << std::string(20, '-') << std::endl;
        std::cout << "response = " << _response << std::endl;
        std::cout << std::string(20, '-') << std::endl;
        waitpid(_pid, NULL, 0);
        send(_fd, _response.c_str(), _response.size(), 0);
    }
}