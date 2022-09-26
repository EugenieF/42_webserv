#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include <ctime>

# define WEBSERV_VERSION	"Cutie Webserv 1.0"

# include "Request.hpp"
# include "MimeType.hpp"

extern StatusCode	g_statusCode;
extern MimeType		g_mimeType;

class   Response
{
	public:
	/***********************      MEMBER TYPES      *********************/
		// typedef Block::t_method						t_method;
		typedef void (Response::*httpMethod)(std::string&);
		typedef std::map<t_method, httpMethod>		listOfHttpMethodsFunct;
		typedef Request::listOfHeaders				listOfHeaders;


    private:
	/**********************     MEMBER VARIABLES     ********************/
		listOfHttpMethodsFunct				_httpMethods;

		Block*								_server;
		Request*							_request;
		std::string							_response;
		t_statusCode						_statusCode;
		std::string							_body;
		Block*								_matchingBlock;
		listOfHeaders						_headers;
		t_method							_method;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*------    Main    ------*/
        Response(Block* server, Request* request);
        Response(const Response& other);
        ~Response();
        Response&    						operator=(const Response& other);

		void								generateResponse();

						/*-------   Getter  ------*/
		Request*							getRequest() const;
		std::string							getResponse() const;
		t_statusCode						getStatusCode() const;
		std::string							getStatusCodeStr() const;
		std::string							getBody() const;

						/*-------   Setter  ------*/
		void								setStatusCode(t_statusCode status);

	private:
						/*-------   Init    ------*/
		void								_initHttpMethods();


        std::string							_generateErrorPage();
		void								_fillResponseLine();
		void								_fillHeaders();
		void								_selectMatchingBlock();
		std::string							_buildPath();

						/*-------  Methods   ------*/
		void								_processMethod();
		void								_getMethod(std::string& path);
		void								_postMethod(std::string& path);
		void								_deleteMethod(std::string& path);

						/*-------   Utils    ------*/
		bool								_uploadPathDirective();
		std::string							_getDateHeader();
		std::string							_getContentTypeHeader();
		bool								_requestIsValid();
		bool								_checkBodyLimit();
		
};

#endif
