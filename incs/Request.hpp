#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <sys/time.h>

#include "Block.hpp"
#include "Response.hpp"

class   Request
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef void (Request::*parsingFunction)(std::string&);
		typedef std::vector<parsingFunction>		listOfParsingFunctions;

    private:
	/**********************     MEMBER VARIABLES     ********************/
		listOfParsingFunctions		_parsingFunct;
		Block*						_server;
		std::string					_request;
		t_method					_method;
		std::string					_uri;
		std::string					_httpProtocol;
		// std::string					_body;
		// size_t						_bodySize;
		// struct timeval				_time;
		t_statusCode				_statusCode;
		bool						_requestIsValid;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*------    Main    ------*/
        Request();
        Request(Block* server, const std::string& buffer);
        Request(const Request& other);
        ~Request();
        Request&    	operator=(const Request& other);

						/*------   Parsing  ------*/
		bool			parseRequest();
		void			completeRequest(const std::string& buffer);

	private:
		void			_parseMethod(std::string &requestLine);
		void			_parseUri(std::string &requestLine);
		void			_parseHttpProtocol(std::string &requestLine);
		void			_parseHeaders();

	public:
						/*------   Getter  ------*/
		std::string		getRequest() const;
		t_method		getMethod() const;
		std::string		getUri() const;
		std::string		getHttpProtocol() const;

	private:
	/**********************  PRIVATE MEMBER FUNCTIONS  ******************/

						/*------   Utils  ------*/
		void			_initParsingFunct();
		void			_requestIsInvalid(t_statusCode code);
		std::string		_getNextWord(std::string& line);

};

#endif