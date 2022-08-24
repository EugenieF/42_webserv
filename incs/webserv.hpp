#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* Cpp libraries */
// # include <cstdio>
// # include <iostream>

/* C libraries */
// # include <unistd.h>
// # include <stdlib.h>
// # include <string.h>

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <fcntl.h>

# include "Parser.hpp"

# define PORT	8080

typedef enum e_define
{
	FAILURE		= 0,
	SUCCESS 	= 1,
	BACKLOG		= 10,
}	t_define;

class Webserv
{
	private:
		Parser	_parser;

	public:
		Webserv(std::string configFile);
		~Webserv();
		void			displayServers();
};

#endif