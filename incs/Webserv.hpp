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
# include "EpollInstance.hpp"
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
		typedef	std::map<Server*, Block*>			serverMap;
		typedef	serverMap::iterator					it;
		typedef	Parser::listOfServers				listOfServers;

	/**********************     MEMBER VARIABLES     ********************/
		Parser							_parser;
		serverMap						_servers;
		EpollInstance					_epoll;

	public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
		Webserv(std::string configFile, char* const* env);
		//Webserv(const Webserv& other);
		~Webserv();
		//Webserv&						operator=(const Webserv& other);

						/*-------   Getter   ------*/
		Parser							getParser() const;
		const serverMap&				getServers() const;
		const EpollInstance&			getEpoll() const;
		const listOfServers&			getConfigServers() const;
		int								getNbOfServers() const;

						/*-------   Display   ------*/
		void							displayServers() const;
	
	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*-------    Setup   ------*/
		void							_setupServerMap(const listOfServers&
										configServers, char* const* env);
};

#endif
