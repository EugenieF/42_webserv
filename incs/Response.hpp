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
		// typedef Block::t_method						t_method;
		typedef void (Response::*httpMethod)(std::string&);
		typedef std::map<t_method, httpMethod>		listOfHttpMethodsFunct;
		typedef Request::listOfHeaders				listOfHeaders;
		typedef Block::listOfStrings				listOfStrings;

    private:
	/**********************     MEMBER VARIABLES     ********************/
		listOfHttpMethodsFunct			_httpMethods;

		Block*							_server;
		Request*						_request;
		std::string						_response;
		t_statusCode					_statusCode;
		std::string						_body;
		Block*							_matchingBlock;
		listOfHeaders					_headers;
		t_method						_method;
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

						/*-------   Getter  ------*/
		Request*						getRequest() const;
		std::string						getResponse() const;
		t_statusCode					getStatusCode() const;
		std::string						getStatusCodeStr() const;
		std::string						getBody() const;

						/*-------   Setter  ------*/
		void							setStatusCode(t_statusCode status);
		void							setStatusCode(int status);

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/
						/*-------   Init    ------*/
		void							_initHttpMethods();

						/*-------  Generate  ------*/
        std::string						_generateErrorPage();
		void							_fillResponseLine();
		void							_fillHeaders();
		std::string						_buildPath();
		void							_processMethod();

						/*------  Get Method  -----*/
		void							_getMethod(std::string& path);
		void							_readFileContent(std::string& path);
		bool							_searchOfIndexPage(listOfStrings indexes, std::string* path);
		bool							_foundIndexPage(DIR* dir, std::string indexPage);
		void							_handleRedirection();
		void							_generateAutoindex(std::string& path);

						/*------  Post Method -----*/
		void							_postMethod(std::string& path);
		void							_handleUploadFile();
		void							_handleCgi();
		void							_writeFileContent(std::string& path);

						/*-----  Delete Method ----*/
		void							_deleteMethod(std::string& path);

						/*--------   Error  -------*/
		void							_throwErrorMsg(t_statusCode errorCode, const std::string& message);
		void							_throwErrorMsg(const std::string& message);
		t_statusCode					_getErrorCodeWithErrno();

						/*-------   Utils    ------*/
		bool							_hasUploadPathDirective();
		std::string						_getDateHeader();
		std::string						_getContentTypeHeader();
		bool							_requestIsValid();
		void							_checkBodyLimit();
		std::string						_buildFilePath(std::string& path);


};

#endif
