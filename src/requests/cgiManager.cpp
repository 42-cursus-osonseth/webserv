#include "cgiManager.hpp"
#include "request.hpp"
#include <stdio.h>

#include <sys/types.h>
#include <signal.h>

cgiManager::~cgiManager()
{
}
void cgiManager::initPostEnv(client &client)
{

    std::ostringstream oss;
    oss << client.getContentLenght();
    std::string lenStr = oss.str();

    if (!client.getUploadPath().empty())
        _upload = "UPLOAD_PATH=" + client.getUploadPath();
    _contentLength = "CONTENT_LENGTH=" + lenStr;
    _contentType = "CONTENT_TYPE=" + client.getContentType();
    _filename = "FILENAME=" + client.getFilename();
    _env[0] = (char *)"REQUEST_METHOD=POST";
    _env[1] = (char *)_contentType.c_str();
    _env[2] = (char *)_contentLength.c_str();
    _env[3] = (char *)_filename.c_str();
    _env[4] = (char *)_upload.c_str();
    if (client.getIsChunk())
        _env[5] = (char *)"TRANSFER_ENCODING=chunked";
    else
        _env[5] = NULL;
    _env[6] = NULL;
}
void cgiManager::initGetenv()
{
    _query = "QUERY_STRING=" + _query;
    _env[0] = (char *)"REQUEST_METHOD=GET";
    _env[1] = (char *)_query.c_str();
    _env[2] = NULL;
    _env[3] = NULL;
    _env[4] = NULL;
    _env[5] = NULL;
    _env[6] = NULL;
}

cgiManager::cgiManager(Request &req, client &client) : _fd(client.getFd()), _body(req.getterBody()), _method(client.getMethod()), _query(req.getterQuery()), _path(client.getPath()), _req(req)
{
    _sv_in[0] = -1;
    _sv_in[1] = -1;
    _sv_out[0] = -1;
    _sv_out[1] = -1;
    _args[0] = (char *)_path.c_str();
    _args[1] = NULL;
    if (_method == "POST")
        initPostEnv(client);
    else
        initGetenv();
    memset(_buffer, 0, sizeof(_buffer));
}

void cgiManager::executePostRequest(client &client)
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
        int resp;
        int start = time(NULL);
        close(_sv_in[0]);
        close(_sv_out[1]);
        write(_sv_in[1], _body.c_str(), _body.size());
        close(_sv_in[1]);
        while (true)
        {
            pid_t result = waitpid(_pid, &resp, WNOHANG);
            if (result == -1)
            {
                _req.execThrow(INTERNAL_SERVER_ERROR);
            }
            else if (result > 0)
            {
                if (WIFEXITED(resp))
                {
                    int status = WEXITSTATUS(resp);
                    if (status != 0)
                        _req.execThrow(status < 200 ? status + 256 : status);
                    break;
                }
            }
            if (time(NULL) - start >= TIMEOUT_SECONDS)
            {
                kill(_pid, SIGKILL);
                waitpid(_pid, NULL, 0);
                _req.execThrow(REQUEST_TIMEOUT);
            }
            usleep(10000);
        }
    }
    while ((_bytesRead = read(_sv_out[0], _buffer, sizeof(_buffer) - 1)) > 0)
        _response.append(_buffer, _bytesRead);
    close(_sv_out[0]);
    send(_fd, _response.c_str(), _response.size(), 0);
    if ((client.getContentType() == "multipart/form-data" || client.getIsChunk()) && !client.getBodyFullyRead())
        return;
}
void cgiManager::executeGetRequest()
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
        int resp;
        int start = time(NULL);

        close(_sv_in[0]);
        while (true)
        {
            pid_t result = waitpid(_pid, &resp, WNOHANG);
            if (result == -1)
            {
                _req.execThrow(INTERNAL_SERVER_ERROR);
            }
            else if (result > 0)
            {
                if (WIFEXITED(resp))
                {
                    int status = WEXITSTATUS(resp);
                    if (status != 0)
                        _req.execThrow(status < 200 ? status + 256 : status);
                    break;
                }   
                else
                {
                    std::cerr << "CGI script terminated abnormally" << std::endl;
                    throw std::runtime_error("CGI script failed");
                }
            }
            if (time(NULL) - start >= TIMEOUT_SECONDS)
            {
                kill(_pid, SIGKILL);
                waitpid(_pid, NULL, 0);
                _req.execThrow(REQUEST_TIMEOUT);
            }
            usleep(10000);
        }
    }

    while ((_bytesRead = read(_sv_in[1], _buffer, sizeof(_buffer) - 1)) > 0)
        _response.append(_buffer, _bytesRead);
    close(_sv_in[1]);
    send(_fd, _response.c_str(), _response.size(), 0);
}

void cgiManager::execute(client &client)
{
    if (_method == "POST")
        executePostRequest(client);
    else
        executeGetRequest();
}
