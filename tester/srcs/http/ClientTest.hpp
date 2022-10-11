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
	public:
		std::string		request;

	private:
		std::string		_ip;
		int				_port;
        int				_sock;

	public:
		ClientTest(): _ip("127.0.0.1"), _port(8000), _sock(0) {}
		~ClientTest() {}

		void init(std::string ip, int port)
		{
			_ip = ip;
			_port = port;
		}

		void sendRequest(std::string request)
		{
    		struct sockaddr_in serv_addr;

    		_sock = socket(AF_INET, SOCK_STREAM, 0);
			if (_sock < 0)
       			throwErrorMsg("Socket creation error");
			memset(&serv_addr, '0', sizeof(serv_addr));
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(_port);
			if(inet_pton(AF_INET, _ip.c_str(), &serv_addr.sin_addr)<=0)
				throwErrorMsg("Invalid address/ Address not supported");
			if (connect(_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
				throwErrorMsg("Connection Failed");
			send(_sock, request.c_str(), strlen(request.c_str()) , 0 );
		}

		std::string	readResponse()
		{
    		char buffer[2048] = {0};
			read(_sock , buffer, 1024);
			close(_sock);
			return (std::string(buffer));
		}

		void	throwErrorMsg(std::string msg)
		{
			throw std::runtime_error(msg);
		}
};

#endif
