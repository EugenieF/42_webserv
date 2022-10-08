#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* C libraries */
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <fcntl.h>

# include "Parser.hpp"
# include "Server.hpp"
# include "utils.hpp"

# define NL '\n'
# define PORT	8080

/******************************************************************************/
/*                                  ENUM                                      */
/******************************************************************************/

typedef enum e_define
{
	FAILURE		= 0,
	SUCCESS 	= 1,
	BACKLOG		= 10,
}	t_define;

/******************************************************************************/
/*                               CLASS WEBSERV                                */
/******************************************************************************/

class Webserv
{
	private:
	/**********************     MEMBER VARIABLES     ********************/
		Parser							_parser;

	public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
		Webserv();
		Webserv(std::string configFile);
		Webserv(const Webserv& other);
		~Webserv();
		Webserv&						operator=(const Webserv& other);

						/*-------   Parser   ------*/
		void							parse(std::string configFile);

						/*-------   Getter   ------*/
		Parser::listOfServers			getServers();
		int								getNbOfServers();
		Parser							getParser() const;

						/*-------   Display   ------*/
		void							displayServers();
};

#endif