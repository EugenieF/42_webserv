#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <sys/time.h>

#include "Block.hpp"

class   Request
{
    private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string		_request;
		t_method		_method;
		std::string		_uri;
		std::string		_httpProtocol;
		std::string		_body;
		size_t			_bodySize;
		struct timeval	_time;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*------    Main    ------*/
        Request();
        Request(const std::string& buffer);
        Request(const Request& other);
        ~Request();
        Request&    	operator=(const Request& other);

						/*------   Parsing  ------*/
		void			parseRequest();
		void			completeRequest(const std::string& buffer);

		void			parseMethod();
		void			parseUri();
		void			parseHttpProtocol();
		void			parseHeaders();

						/*------   Getter  ------*/
		std::string		getRequest() const;
		t_method		getMethod() const;
		std::string		getUri() const;
		std::string		getHttpProtocol() const;

	private:
		void			requestIsInvalid();

};

#endif