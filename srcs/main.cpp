/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 15:30:51 by etran             #+#    #+#             */
/*   Updated: 2022/09/26 14:05:52 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

//bool	createSocket(int *socketFd, struct sockaddr_in socketAddress)
//{
//	int yes = 1;
//
//	/* int socket(int domain, int type, int protocol) */
//	*socketFd = socket(AF_INET, SOCK_STREAM, 0);
//	if (*socketFd < 0) 
//	{
// 		perror("cannot create socket"); 
//		return (FAILURE); 
//	}
//
//	// fcntl(socketFd, F_SETFL, O_NONBLOCK);
//
//	/* int setsockopt(int socket, int level, int optname, const void *optval, socklen_t optlen) */
//	if (setsockopt(*socketFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
//	{
//		perror("setsockopt failed");
//		return (FAILURE);
//	}
//
//	/* int bind(int socket, const struct sockaddr *address, socklen_t address_len) */
//	if (bind(*socketFd, (struct sockaddr *)&socketAddress, sizeof(socketAddress)) < 0) 
//	{
//		close(*socketFd);
//		perror("bind failed");
//		return (FAILURE);
//	}
//
//	/* int listen(int socket, int backlog) */
//	if (listen(*socketFd, BACKLOG) < 0)
//	{ 
//		perror("In listen");
//		return (FAILURE);
//	}
//	return (SUCCESS);
//}

//bool	receiveMessage(int connectionFd)
//{
//	char buffer[30000];
//	int retRecv;
//
//	memset((void *)buffer, 0, 30000);
//	retRecv = recv(connectionFd, buffer, 30000, 0);
//	if (retRecv < 0)
//	{
//		perror("recv failed");            
//		return (FAILURE);        
//	}
//	std::cout << buffer << std::endl;
//	return (SUCCESS);
//}
//
//bool	sendMessage(int connectionFd)
//{
//	char response[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 13\n\nHello world !";
//
//	send(connectionFd, response, strlen(response), 0);
//	std::cout << "------------------Hello message sent-------------------" << std::endl;
//	return (SUCCESS);
//}
//
//bool	waitForConnection(int socketFd, struct sockaddr_in socketAddress)
//{
//	int connectionFd;
//	int addressLength;
//	addressLength = sizeof(socketAddress);
//
//	while (1)
//	{
//		std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;
//
//		/* int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len) */
//		connectionFd = accept(socketFd, (struct sockaddr *)&socketAddress, (socklen_t*)&addressLength);
//		if (connectionFd < 0)
//		{
//			perror("In accept");            
//			return (FAILURE);        
//		}
//		receiveMessage(connectionFd);
//		sendMessage(connectionFd);
//		close(connectionFd);
//	}
//	return (SUCCESS);
//}

//bool	runServer(void)
//{
//	int socketFd;
//	struct sockaddr_in socketAddress;
//	memset((char *)&socketAddress, 0, sizeof(socketAddress));
//	socketAddress.sin_family = AF_INET;
//	socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
//	socketAddress.sin_port = htons(PORT);
//
//	createSocket(&socketFd, socketAddress);
//	waitForConnection(socketFd, socketAddress);
//	
//	close(socketFd);
//	return (SUCCESS);
//}

int main(int argc, char **argv)
{
	std::string configFile("tester/testFiles/valid/default.conf");

	if (argc != 2)
	{
		std::cerr << "usage : ./webserv [a configuration file]" << std::endl;
		if (argc > 2)
			return (EXIT_FAILURE);
		std::cout << "Using default configuration file" << std::endl;
	}
	else
		std::string configFile(argv[1]);
	try
	{
		Webserv	webserv(configFile);
		setupSignal(INIT_SIGNAL);
		Server	serv(*webserv.getServers()[0], webserv.getServers());
		serv.launchServer();
		// webserv.displayServers();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cerr << "Errno value: " << errno << NL;
		setupSignal(RESET_SIGNAL);
		return (EXIT_FAILURE);
	}
	setupSignal(RESET_SIGNAL);
	return (EXIT_SUCCESS);
}
