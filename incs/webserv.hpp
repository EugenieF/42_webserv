#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* Cpp libraries */
# include <cstdio>
# include <iostream>

/* C libraries */
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <fcntl.h>

typedef enum e_define
{
	FAILURE	= 0,
	SUCCESS = 1,
	PORT	= 8080,
	BACKLOG	= 10,
}	t_define;

#endif