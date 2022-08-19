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
	FAILURE		= 0,
	SUCCESS 	= 1,
	PORT		= 8080,
	BACKLOG		= 10,
}	t_define;

#define RESET	"\033[0m"
#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define BLUE	"\033[1;34m"
#define WHITE	"\033[1;37m"
#define ORANGE	"\033[1;38;5;208m"
#define GREY	"\033[0;38;5;241m"

#endif