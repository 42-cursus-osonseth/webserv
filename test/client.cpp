#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

#define PORT 8080

int main()
{
	int sock = 0;long valread;
	struct sockaddr_in serv_addr;
	const char *hello = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cerr << "\n Socket crearion failed  \n";
		return -1;
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
	{
		std::cerr << "\nInvalid address/ Address not supported \n";
		return -1;
	}
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::cerr << "\nConnection failed \n";
		return -1;
	}
	send(sock, hello, strlen(hello), 0);
	std::cout << "Hello message sent\n";
	(void)valread;
	(void)buffer;
	valread = read(sock, buffer, 1024);
	if (valread > 0)
		std::cout << buffer << "\n";
	else
		std::cerr << "Read error\n";
	return 0;
}