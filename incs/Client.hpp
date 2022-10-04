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
		Block*					_runningServer;
        Request*            	_request;
        Response*           	_response;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
        Client();
        Client(Block* server);
        Client(Client const& other);
        ~Client();
        Client&             	operator=(Client const& other);

        t_requestStatus     	parseRequest(std::string const& buffer);
        std::string         	generateResponse();
		void			    	clear();
        Block*              	selectVirtualServer();

						/*-------   Getter   ------*/
        Block*      			getRunningServer() const;
        Request*            	getRequest() const;
        Response*           	getResponse() const;

	private :
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/
		void					_parseHostHeader(std::string const& hostHeader, std::string& host, int& port);
		bool					_matchingServerName(listOfStrings serverNames, int listeningPort);
};

#endif
