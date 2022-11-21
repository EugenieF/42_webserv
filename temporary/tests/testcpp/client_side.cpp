
#include <exception>
#include <stdlib.h>
#include<netdb.h>
#include<unistd.h>
#include<sys/socket.h>
#include <stdio.h>
#include<string.h>
#include<errno.h>
#include<arpa/inet.h>
# include <iostream>
# define DEBUG(x) printf(" ### Debug ### : %s\n", x)

# define BUFSIZE 2048
# define NL '\n'

//Client side

void printerror() {
	fprintf(stderr, "error %d: %s\n",errno, strerror(errno));
}

int main(int ac, char** av, char** env) {
	(void)env;
	if (ac != 4) {
		fprintf(stderr, "Error, wrong nb argument: ./client [host] [port] [msg]\n");
		return (0);
	}
	int port = atoi(av[2]);
	int s = socket(PF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		printerror();
		return (1);
	}
	DEBUG("Passed socket");

	struct sockaddr_in servaddr;
	int len = sizeof(servaddr);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_addr.s_addr = inet_addr(av[1]);
	servaddr.sin_port = htons(port);

	//DEBUG
	char addr_buf[INET_ADDRSTRLEN + 1];
	memset(&addr_buf, 0, INET_ADDRSTRLEN + 1);
	if (inet_ntop(AF_INET, &servaddr.sin_addr, addr_buf, INET_ADDRSTRLEN) == 0) 
		throw std::runtime_error("inet_ntop");
	std::cout	<< "fd: " << s << NL
				<< "host addr: " << addr_buf << NL
				<< "'port: " << port << NL;
	//====

	if (connect(s, (struct sockaddr*)&servaddr, len) == -1) {
		printerror();
		close(s);
		return (1);
	}
	DEBUG("Passed connect");

	const char* msg = av[3];
	const int len_msg = strlen(msg);
	if (send(s, msg, len_msg, 0) < 0) {
		printerror();
		close(s);
		return (1);
	}
	DEBUG("Passed send");

	char buf[BUFSIZE + 1];
	memset(buf, 0, BUFSIZE + 1);
	if (recv(s, buf, BUFSIZE, 0) < 0) {
		printerror();
		close(s);
		return (1);
	}
	DEBUG("Passed recv");
	printf("Message received:\n%s\n", buf);
	close(s);
	return (0);
}
