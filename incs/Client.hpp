#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include "Parser.hpp"
# include "Response.hpp"
# include "utils.hpp"


/******************************************************************************/
/*                                CLASS CLIENT                                */
/******************************************************************************/

class Client
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef Parser::listOfServers		listOfServers;
		typedef Block::listOfStrings		listOfStrings;
		typedef	std::pair<Server*, Block*>	serverMapNode;

    private:
	/**********************     MEMBER VARIABLES     ********************/
		int						_sockfd;
		serverMapNode			_runningServer;
        Request*            	_request;
        Response*           	_response;
		Env						_env;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
		Client(serverMapNode server, int sockfd, const Env& env);
        Client(Client const& other);
        ~Client();
        Client&             	operator=(const Client& other);

						/*-------  Request  ------*/
        t_requestStatus     	parseRequest(const std::string& buffer);

						/*-------  Response ------*/
		void					generateResponse();

						/*-------   Getter   ------*/
        serverMapNode  			getRunningServer() const;
        Request*            	getRequest() const;
        Response*           	getResponse() const;
		int						getFd() const;
		const Env&				getEnv() const;
		Env&					getEnv();

						/*-------    Utils   ------*/
		void			    	clear();
		void					displayConnectionInfos();

	private :
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*-------  Response ------*/
        Block*              	_selectVirtualServer();
		bool					_matchingServerName(
									const listOfStrings& serverNames,
										int listeningPort, Block* currentServer);
};

#endif
