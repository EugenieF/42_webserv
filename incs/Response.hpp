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
# include "Env.hpp"
# include "CgiHandler.hpp"

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
		int								_fd;
		std::string						_buildpath;

		Env*							_env; // Shares same env than client
		std::string						_cgiscript;
		std::string						_cgiextra;
		std::string						_cgiquery;
		std::string						_cgipath;

    public:
	/**********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*------    Main    ------*/
		Response(Block* server, Request* request, Env& env);
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
		int								getFd() const;
		std::string						getBuildPath() const;

		std::string						getCgiProgram() const;
		std::string						getCgiName() const;
		std::string						getCgiExtra() const;
		std::string						getCgiQuery() const;
		const Env&						getEnv() const;

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
		std::string						_getBoundary(std::string contentType);
		std::string						_getFilename(const std::string& content);

						/*-----  Delete Method ----*/
		void							_runDeleteMethod(std::string& path);

						/*-------   Path    ------*/
		std::string						_buildPath();
		void							_handleDirectoryPath(std::string* path);
		bool							_hasUploadPathDirective();
		bool							_searchOfIndexPage(const listOfStrings& indexes, std::string* path);
		bool							_foundIndexPage(DIR* dir, const std::string& indexPage);
		void							_generateAutoindex(const std::string& path);

						/*-----   Redirection  ----*/
		void							_handleRedirection();

						/*--------   Error  -------*/
		void							_throwErrorMsg(t_statusCode errorCode, const std::string& message);
		void							_throwErrorMsg(const std::string& message);
		t_statusCode					_findErrorCode();

						/*-------  Headers   ------*/
		std::string						_getDateHeader();
		std::string						_getContentTypeHeader();
		std::string						_getConnectionHeader();
		void							_fillExtraHeader();

						/*-------   Utils    ------*/
		bool							_requestIsValid();
		void							_checkBodyLimit();
		bool							_isMultipartFormRequest();
		bool							_isCgi(const std::string& path);

						/*-------    Cgi     ------*/
		size_t							_parsePosCgiExtension(std::string path);
		void							_parseCgiUrl(size_t pos);
		void							_fillCgiMetavariables();
		std::string						_translateCgiName() const;

	/***************************     BONUS     *************************/
	#ifdef COOKIE
	public:
        Response(Block* server, Request* request, Env& env, Cookie& cookies);
		Cookie&							getCookies();
		const Cookie&					getCookies() const;

	private:
		Cookie							_cookies;
		void							_fillCookieHeader();
	#endif
};

#endif
