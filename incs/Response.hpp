#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include <ctime>
# include <cstdio>
# include <cerrno>
# include <iostream>
# include <fstream>

# include <dirent.h>

# define WEBSERV_VERSION	"Cutie Webserv 1.0"

# include "Request.hpp"
# include "MimeType.hpp"
# include "Autoindex.hpp"

extern StatusCode	g_statusCode;
extern MimeType		g_mimeType;

class   Response
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef void (Response::*httpMethod)(std::string&);
		typedef std::map<t_method, httpMethod>		listOfHttpMethodsFunct;
		typedef Request::listOfHeaders				listOfHeaders;
		typedef Block::listOfStrings				listOfStrings;

    private:
	/**********************     MEMBER VARIABLES     ********************/
		Block*							_server;
		Block*							_matchingBlock;
		Request*						_request;
		std::string						_response;
		t_statusCode					_statusCode;
		t_method						_method;
		listOfHeaders					_headers;
		std::string						_body;
		listOfHttpMethodsFunct			_httpMethods;
		std::string						_locationPath;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*------    Main    ------*/
        Response(Block* server, Request* request);
        Response(const Response& other);
        ~Response();
        Response&    					operator=(const Response& other);

						/*-------  Generate  ------*/
		void							generateResponse();

						/*-------   Setter  ------*/
		void							setStatusCode(t_statusCode status);
		void							setStatusCode(int status);

						/*-------   Getter  ------*/
		Block*							getServer() const;
		Block*							getMatchingBlock() const;
		Request*						getRequest() const;
		std::string						getResponse() const;
		t_statusCode					getStatusCode() const;
		std::string						getStatusCodeStr() const;
		t_method						getMethod() const;
		listOfHeaders					getHeaders() const;	
		std::string						getBody() const;
		listOfHttpMethodsFunct			getHttpMethods() const;
		std::string						getLocationPath() const;

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/
						/*-------   Init    ------*/
		void							_initHttpMethods();

						/*-------  Generate  ------*/
        std::string						_generateErrorPage();
		void							_processMethod();
		void							_fillResponseLine();
		void							_fillHeaders();
		void							_fillErrorBody();

						/*------  Get Method  -----*/
		void							_runGetMethod(std::string& path);
		void							_readFileContent(const std::string& path);

						/*------  Post Method -----*/
		void							_runPostMethod(std::string& path);
		void							_writeFileContent(const std::string& path, const std::string& content);
		void							_handleUploadFile();
		void							_handleCgi();
		void							_handleMultipartContent(const std::string& path, std::string content);
		std::string						_getBoundary(const std::string& contentType);
		std::string						_getFilename(const std::string& content);

						/*-----  Delete Method ----*/
		void							_runDeleteMethod(std::string& path);

						/*--------   Index  -------*/
		bool							_searchOfIndexPage(const listOfStrings& indexes, std::string* path);
		bool							_foundIndexPage(DIR* dir, const std::string& indexPage);
		void							_generateAutoindex(const std::string& path);

						/*-----   Redirection  ----*/
		void							_handleRedirection();

						/*--------   Error  -------*/
		void							_throwErrorMsg(t_statusCode errorCode, const std::string& message);
		void							_throwErrorMsg(const std::string& message);
		t_statusCode					_getErrorCodeWithErrno();

					/*-------  Headers   ------*/
		std::string						_getDateHeader();
		std::string						_getContentTypeHeader();
		std::string						_getConnectionHeader();

						/*-------   Utils    ------*/
		std::string						_buildPath();
		bool							_hasUploadPathDirective();
		bool							_requestIsValid();
		void							_checkBodyLimit();
		bool							_isMultipartFormRequest();

};

#endif
