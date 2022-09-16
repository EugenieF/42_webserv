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
		bool			parseRequest();
		void			completeRequest(const std::string& buffer);

		void			parseMethod(std::string &requestLine);
		void			parseUri(std::string &requestLine);
		void			parseHttpProtocol(std::string &requestLine);
		void			parseHeaders();

						/*------   Getter  ------*/
		std::string		getRequest() const;
		t_method		getMethod() const;
		std::string		getUri() const;
		std::string		getHttpProtocol() const;

	private:
		void			_requestIsInvalid();
		std::string		_getNextWord(std::string& line);

};

#endif