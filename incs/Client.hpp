#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Parser.hpp"
# include "Response.hpp"

class Client
{
    public:
        typedef Parser::listOfServers   listOfServers;

    private:
	/**********************     MEMBER VARIABLES     ********************/
        listOfServers       _servers;
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
        Block*              findMatchingServer();

						/*-------     Getter      ------*/
        listOfServers       getServers() const;
        Request*            getRequest() const;
        Response*           getResponse() const;

	private :
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

};

#endif
