#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* C libraries */
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <fcntl.h>

// # include "utils.hpp"
# include "Parser.hpp"
# include "EpollInstance.hpp"

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
		//typedef	std::pair<Server, Block*>			serverMapNode;
		typedef	serverMap::iterator					it;
		typedef	Parser::listOfServers				listOfServers;

	/**********************     MEMBER VARIABLES     ********************/
		Parser							_parser;
		char* const*					_env;
		serverMap						_servers;
		EpollInstance					_epoll;

	public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
		Webserv();
		Webserv(std::string configFile, char* const* env);
		Webserv(const Webserv& other);
		~Webserv();
		Webserv&						operator=(const Webserv& other);

						/*-------    Run     ------*/
		void							parse(std::string configFile);
		void							run();

						/*-------   Getter   ------*/
		Parser							getParser() const;
		serverMap						getServers() const;
		EpollInstance					getEpoll() const;
		char* const*					getEnv() const;
		listOfServers					getConfigServers();
		int								getNbOfServers();

						/*-------   Display   ------*/
		void							displayServers();
	
	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*-------    Setup   ------*/
		void							_setupServerMap(listOfServers configServers);
};

#endif
