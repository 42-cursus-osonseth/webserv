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
// cgiManager::cgiManager(std::string path, int fd) : _path(path), _response(""), _fd(fd), _pid(-1), _bytesRead(0)
// {
//     _pipefd[0] = -1;
//     _pipefd[1] = -1;
//     _args[0] = const_cast<char *>(_path.c_str());
//     _args[1] = NULL;
//     _env[0] = (char *)"REQUEST_METHOD=GET";
//     _env[1] = (char *)"QUERY_STRING=test=123";
//     _env[2] = (char *)"CONTENT_TYPE=text/html";
//     _env[3] = (char *)"CONTENT_LENGTH=0";
//     _env[4] = NULL;
//     memset(_buffer, 0, sizeof(_buffer));
// }
cgiManager::cgiManager (Request &req) : _fd(req.getFd())
{
    // std::cout << "BODY = " << req.getterBody() << std::endl;
    // std::cout << "PATH = " << req.getPath() << std::endl;
    // std::cout << "METHOD = " <<req.getMethod() << std::endl;
    // std::cout << " test =  "<< std::hex << (int)req.getterBody()[23] << std::endl;
    _pipefd[0] = -1;
    _pipefd[1] = -1;
    _args[0] = const_cast<char *>(req.getPath().c_str());
    _args[1] = NULL;
    if (req.getMethod() == "POST")
        initPostEnv(req.getterBody().size());
    else
        _env[0] = NULL;
    memset(_buffer, 0, sizeof(_buffer));
}

void cgiManager::execute()
{
    // std::cout << " path dans class cgi = " << _path << std::endl;
    // std::cout << " path dans args[0] = " << _args[0] << std::endl;
    if (pipe(_pipefd) == -1)
        throw std::runtime_error("pipe failed");
    if ((_pid = fork()) == -1)
        throw std::runtime_error("fork failed");
    if (_pid == 0)
    {
        dup2(_pipefd[1], STDOUT_FILENO);
        close(_pipefd[0]);
        close(_pipefd[1]);
        if (execve(_args[0], _args, _env) == -1)
        {
            perror("");
            throw std::runtime_error("execve failed failed");

        }
    }
    else
    {
        close(_pipefd[1]);
        waitpid(_pid, NULL, 0);
        while ((_bytesRead = read(_pipefd[0], _buffer, sizeof(_buffer) - 1)) > 0)
            _response.append(_buffer, _bytesRead);
        close(_pipefd[0]);
        std::cout << "response = " << _response << std::endl;
        send(_fd, _response.c_str(), _response.size(), 0);
    }
}