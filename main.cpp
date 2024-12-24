#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

int main()
{
	struct sockaddr_in address;
	int newSocket;
	int addrlen = sizeof(address);
	const int port = 8080;
	std::memset(reinterpret_cast<char*>(&address), 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	memset(address.sin_zero, 0, sizeof(address.sin_zero));
	int	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
	{
		std::cerr << "socket creation failed " << strerror(socketFd) << '\n';
		exit(EXIT_FAILURE);
	}
	if (bind(socketFd,(struct sockaddr *)&address, sizeof(address)) < 0)
	{
		close(socketFd);
		std::cerr << "bind failed\n";
		exit(EXIT_FAILURE);
	}
	if (listen(socketFd, 3) < 0)
	{
		std::cerr << "In listen\n";
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		std::cout << "\n+++++++ Waiting for new connection +++++++\n\n";
		if ((newSocket = accept(socketFd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
		{
			close(socketFd);
			std::cerr << "In accept\n";
			exit(EXIT_FAILURE);
		}
		char	buffer[30000] = {0};
		int valread = read(newSocket, buffer, 30000);
		if (valread > 0)
			std::cout << buffer << '\n';
		else if (valread < 0)
			std::cerr << "Error reading from socket\n";
		else
			std::cerr << "No data received\n";
		const char* hello = "hello from the server";
		write(newSocket, hello, strlen(hello));
		std::cout << "------------------Hello message sent-------------------\n";
		close(newSocket);
	}
	// struct epoll_event events[EPOLL_EVNT_MAX_SIZE];
	return 0;
}