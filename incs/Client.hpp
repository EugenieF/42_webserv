#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Response.hpp"

class Client
{
    private:
	/**********************     MEMBER VARIABLES     ********************/
        Block*          _runningServer;
        Request*        _request;
        Response*       _response;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------    Main    ------*/
        Client();
        Client(Block* server);
        Client(Client const& other);
        ~Client();
        Client&         operator=(Client const& other);

        t_requestStatus parseRequest(std::string const& buffer);
        std::string     generateResponse();
		void			clear();

						/*-------     Getter      ------*/
        Block*          getRunningServer() const;
        Request*        getRequest() const;
        Response*       getResponse() const;

	private :
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

};

#endif
