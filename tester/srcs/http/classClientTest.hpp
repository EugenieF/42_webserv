#ifndef CLASS_CLIENT_TEST_HPP
# define CLASS_CLIENT_TEST_HPP

#include "../mainTests.hpp"

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

class	ClientTest
{
	private:
		std::string		_ip;
		port			_port;

	public:
		ClientTest(std::string ip, int port): _ip(ip), _port(port) {}
		~ClientTest() {}

		void sendRequest(std::string request)
		{
    		int sock = 0;
    		struct sockaddr_in serv_addr;
    		char buffer[1024] = {0};

    		if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    		{
       		printf("\n Socket creation error \n");
        	return -1;
    		}
			memset(&serv_addr, '0', sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(port);
			if(inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr)<=0)
			{
				printf("\nInvalid address/ Address not supported \n");
				return -1;
			}
			if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
			{
				printf("\nConnection Failed \n");
				return -1;
			}
			send(sock, request.c_str(), strlen(request.c_str()) , 0 );
			printf("Message sent\n");
			read( sock , buffer, 1024);
			printf("%s\n",buffer);
		}
};

#endif
