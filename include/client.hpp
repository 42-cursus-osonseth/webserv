#pragma once 

#include <ctime>
#include <iostream>

#define TIME_OUT 11

class client
{
private:
    int _id;
    std::string _cookies;
    std::time_t _last_request;

public:

    client();
    client(int fd);
    ~client();
    int get_id() const;
    std::string get_cookies() const;
    std::time_t get_last_request_time() const;
    void update_time();
    void print_client() const;

};


