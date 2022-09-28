#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Parser.hpp"
# include "Response.hpp"

# define UNDEFINED_PORT -1

class Client
{
    public:
        typedef Parser::listOfServers   listOfServers;

    private:
	/**********************     MEMBER VARIABLES     ********************/
        listOfServers       _servers;
		Block*				_runningServer;
        Request*            _request;
        Response*           _response;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
        Client();
        Client(listOfServers servers);
        Client(Client const& other);
        ~Client();
        Client&             operator=(Client const& other);

        t_requestStatus     parseRequest(std::string const& buffer);
        std::string         generateResponse();
		void			    clear();
        Block*              findMatchingServer(std::string requestedHost);

						/*-------     Getter      ------*/
        listOfServers       getServers() const;
        Request*            getRequest() const;
        Response*           getResponse() const;

	private :
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/
		void				_parseHostHeader(std::string const& hostHeader, std::string& host, int& port);
		listOfServers		_evaluateServerListen(std::string const& host, int const& port);
		Block*				_evaluateServerNames(listOfServers matchingServers, std::string const& host);
};

#endif
