#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Parser.hpp"
# include "Response.hpp"

class Client
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef Parser::listOfServers	listOfServers;
		typedef Block::listOfStrings	listOfStrings;

    private:
	/**********************     MEMBER VARIABLES     ********************/
		int						_sockfd;
		Block*					_runningServer;
        Request*            	_request;
        Response*           	_response;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
        Client();
        Client(Block* server, int sockfd);
        Client(Client const& other);
        ~Client();
        Client&             	operator=(const Client& other);

						/*-------  Request  ------*/
        t_requestStatus     	parseRequest(const std::string& buffer);

						/*-------  Response ------*/
        std::string         	generateResponse();

						/*-------   Getter   ------*/
        Block*      			getRunningServer() const;
        Request*            	getRequest() const;
        Response*           	getResponse() const;
		int						getFd() const;

						/*-------    Utils   ------*/
		void			    	clear();

	private :
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*-------  Response ------*/
        Block*              	_selectVirtualServer();
		bool					_matchingServerName(const listOfStrings& serverNames, int listeningPort);
};

#endif
