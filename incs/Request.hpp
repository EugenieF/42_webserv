#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <cstdlib>
# include <climits>
# include <sstream>
# include <sys/time.h>

#include "Block.hpp"
#include "StatusCode.hpp"

class   Request
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef Block::t_method					t_method;
		typedef void (Request::*parsingFunction)();
		typedef std::vector<parsingFunction>		listOfParsingFunctions;
		typedef std::map<std::string, std::string>	listOfHeaders;

    private:
	/**********************     MEMBER VARIABLES     ********************/
		Block*						_server;
		std::string					_request;
		t_method					_method;
		std::string					_path;
		std::string					_httpProtocol;
		std::string					_body;
		size_t						_bodySize;
		// struct timeval				_time;
		t_statusCode				_statusCode;
		bool						_requestIsValid;
		listOfParsingFunctions		_parsingFunct;
		listOfHeaders				_headers;
		bool						_chunkedTransfer;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*------    Main    ------*/
        Request();
        Request(Block* server, const std::string& buffer);
        Request(const Request& other);
        ~Request();
        Request&    				operator=(const Request& other);

						/*------   Parsing  ------*/
		bool						parseRequest();
		void						completeRequest(const std::string& buffer);

						/*------   Getter  ------*/
		std::string					getRequest() const;
		t_method					getMethod() const;
		std::string					getPath() const;
		std::string					getHttpProtocol() const;
		t_statusCode				getStatusCode() const;
		std::string					getStatusCodeStr() const;
		bool						getRequestValidity() const;
		listOfParsingFunctions		getParsingFunct() const;
		bool						getChunkedTransfer() const;

						/*------   Display  ------*/
		void						printRequestInfo();

	private:
	/**********************  PRIVATE MEMBER FUNCTIONS  ******************/

						/*------   Parsing  ------*/
		void						_parseMethod();
		void						_parsePath();
		void						_parseHttpProtocol();
		void						_parseHeaders();
		void						_checkHeaders();
		void						_parseBody();

						/*------   Utils  ------*/
		void						_initParsingFunct();
		void						_requestIsInvalid(t_statusCode code);
		size_t						_getNextWord(std::string& word, std::string const& delimiter);
		std::string					_toLowerStr(std::string* str);
		std::string					_trimSpaceStr(std::string *str, const char *toTrim = " \t");

};

#endif