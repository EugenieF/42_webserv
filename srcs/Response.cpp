# include "debug.hpp"
# include "Response.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Response::Response(Block *server, Request* request):
	_server(server),
	_request(request),
	_response(""),
	_statusCode(_request->getStatusCode()),
	_method(_request->getMethod()),
	_body(""),
	_locationPath("")
{
	_initHttpMethods();
}

Response::Response(const Response &other)
{
	*this = other;
}

Response::~Response() {}

Response&	Response::operator=(const Response &other)
{
	if (this != &other)
	{
		_server = other.getServer();
		_matchingBlock = other.getMatchingBlock();
		_request = other.getRequest();
		_response = other.getResponse();
		_statusCode = other.getStatusCode();
		_method = other.getMethod();
		_headers = other.getHeaders();
		_body = other.getBody();
		_httpMethods = other.getHttpMethods();
		_locationPath = other.getLocationPath();
		#ifdef COOKIE
			_cookies = other.getCookies();
		#endif
	}
	return (*this);
}

/******************************************************************************/
/*                                 GENERATE                                   */
/******************************************************************************/

void	Response::_processMethod()
{
	Response::listOfHttpMethodsFunct::const_iterator	ite;
	std::string											path;

	/* Check method validity */
	if (!_matchingBlock->isAllowedMethod(_method))
		throw(METHOD_NOT_ALLOWED);
	path = _buildPath();
	if (path.empty())
		throw(NOT_FOUND);
	/* Finds corresponding http method */
	ite = _httpMethods.find(_method);
	if (ite == _httpMethods.end())
		throw(METHOD_NOT_ALLOWED);
	if (_matchingBlock->redirectDirective())
	{
		/* Do redirection */
		return (_handleRedirection());
	}
	(this->*ite->second)(path);
}

void	Response::generateResponse()
{
	std::string	errorPage;

	_matchingBlock = _server->getMatchingBlock(_request->getPath(), &_locationPath);
	if (_requestIsValid()) /* Handle valid request */
	{
		try
		{
			_checkBodyLimit();
			_processMethod();
		}
		catch(const t_statusCode& errorCode)
		{
			setStatusCode(errorCode);
			_fillErrorBody();
		}
	}
	else /*	Handle invalid request */
	{
		_fillErrorBody();
	}
	_fillResponseLine();
	_fillHeaders();
	_response += _body + "\r\n";
}

/******************************************************************************/
/*                              FILL RESPONSE                                   */
/******************************************************************************/

void	Response::_fillErrorBody()
{
	std::string		errorFile;

	errorFile = _matchingBlock->getErrorPage(_statusCode);
	if (!errorFile.empty())
		return (_readFileContent(errorFile));
	_body = _generateErrorPage();
}

void	Response::_fillResponseLine()
{
	_response = "HTTP/1.1 "
		+ getStatusCodeStr() + " "
		+ g_statusCode[_statusCode]
		+ "\r\n";
}

void	Response::_fillHeaders()
{
	Response::listOfHeaders::const_iterator	ite;

	_headers["Server"] = WEBSERV_VERSION;
	_headers["Content-Type"] = _getContentTypeHeader();
	_headers["Content-Length"] = convertSizeToString(_body.length());
	_headers["Date"] = _getDateHeader();
	_headers["Connection"] = _getConnectionHeader();
	for (ite = _headers.begin(); ite != _headers.end(); ite++)
		_response += ite->first + ": " + ite->second + "\r\n";
	_fillExtraHeader();
	/* An empty line is placed after the series of HTTP headers,
	to divide the headers from the message body */
	_response += "\r\n";
}

/******************************************************************************/
/*                               GET METHOD                                   */
/******************************************************************************/

void	Response::_handleRedirection()
{
	setStatusCode(_matchingBlock->getRedirectCode());
	_headers["Location"] = _matchingBlock->getRedirectUri();
}

void	Response::_readFileContent(const std::string& path)
{
	std::ifstream		file;
	std::stringstream	fileContent;
	
	 /* We try to access the file */
	if (!pathIsAccessible(path))
	{
		throw(NOT_FOUND);
	}
	/* We try to open the file */
	file.open(path.c_str(), std::ifstream::in); 
	if (!file.is_open())
	{
		/* error */
		_throwErrorMsg("Can't open file '" + path + "'");
	}
	fileContent << file.rdbuf();
	_body = fileContent.str();
	file.close();
}

/*  GET method : "Transfer a current representation of the target resource." */
void	Response::_runGetMethod(std::string& path)
{
	std::string		filePath;

	DEBUG("Get method");
	if (_matchingBlock->cgiDirective()) // && check if cgi in request url
	{
		/* process cgi */
		return (_handleCgi());
	}
	_readFileContent(path);
}

// void	Response::_runGetMethod(std::string& path)
// {
// 	std::string		filePath;

// 	DEBUG("Get method");
// 	if (pathIsFile(path))
// 		return (_readFileContent(path));
// 	if (pathIsDirectory(path))
// 	{
// 		/* Directory case */
// 		if (*(path.rbegin()) == '/' && _searchOfIndexPage(_matchingBlock->getIndexes(), &path))
// 			return (_readFileContent(path));
// 		if (*(path.rbegin()) != '/')
// 			path += "/";
// 		if (_matchingBlock->getAutoindex())
// 		{
// 			/* generate autoindex page */
// 			return (_generateAutoindex(path));
// 		}
// 	}
// 	DEBUG("Not Found");
// 	throw(NOT_FOUND);
// }

/******************************************************************************/
/*                            MULTIPART/FORM-DATA                             */
/******************************************************************************/

std::string		Response::_getBoundary(std::string contentType)
{
	std::string	boundary;
	size_t		pos;
	int			lastChar;

	pos = contentType.find("boundary=");
	if (pos == std::string::npos)
		throw(BAD_REQUEST);
	contentType.erase(0, pos + 9);
	boundary = contentType.substr(0, contentType.find("\r\n"));
	trimSpacesEndStr(&boundary);
	lastChar = boundary.length() - 1;
	if (boundary[0] == '\"' && boundary[lastChar] == '\"')
	{
		boundary.erase(0, 1);
		boundary.erase(lastChar - 1);
	}
	if (boundary.length() > 70)
		throw(BAD_REQUEST);
	boundary = "--" + boundary + "\r\n"; 
	return (boundary);
}

std::string		Response::_getFilename(const std::string& content)
{
	size_t		pos;
	std::string	contentDisposition;
	std::string	filename;

	pos = content.find("Content-Disposition:");
	if (pos == std::string::npos)
		throw(BAD_REQUEST);
	contentDisposition = content.substr(pos, content.find("\r\n"));
	pos = contentDisposition.find("filename=\"");
	if (pos == std::string::npos)
		return ("");
	contentDisposition.erase(0, pos + 10);
	filename = contentDisposition.substr(0, contentDisposition.find("\""));
	return (filename);
}

/*
Content-Type: multipart/form-data; boundary=${Boundary}

--${Boundary}
Content-Disposition: form-data; name="name of pdf"; filename="pdf-file.pdf"
Content-Type: application/octet-stream

bytes of pdf file
--${Boundary}
...
*/

void	Response::_handleMultipartContent(const std::string& path, std::string body)
{
	std::string	boundary;
	std::string	filename;
	std::string	fileContent;

	boundary = _getBoundary(_request->getHeader("content-type"));
	while (body.find(boundary) != std::string::npos)
	{
		body.erase(0, body.find(boundary) + boundary.length());
		filename = _getFilename(body);
		body.erase(0, body.find("\r\n") + 2);
		body.erase(0, body.find("\r\n\r\n") + 4);
		if (filename != "")
		{
			fileContent = body.substr(0, body.find("\r\n"));
			std::cout << GREEN << "filepath = " << path + filename << RESET << std::endl;
			_writeFileContent(path + filename, fileContent);
		}
	}
}

/******************************************************************************/
/*                               POST METHOD                                  */
/******************************************************************************/

void	Response::_writeFileContent(const std::string& path, const std::string& content)
{
	std::ofstream	file;

	if (!pathIsAccessible(path))
	{
		/* New file will be created */
		setStatusCode(CREATED);
	}
	file.open(path.c_str(), std::ofstream::app);
	if (!file.is_open())
	{
		/* error */
		_throwErrorMsg("Can't open file '" + path + "'");
	}
	file << content;
	if (file.bad())
	{
		/* error */
		_throwErrorMsg("An error occurred while writing '" + path + "'");
	}
	file.close();
	if (_statusCode == CREATED)
		displayMsg(" 📝 File " + path + " was successfully created", LIGHT_GREEN);
	else
		displayMsg(" 📝 File " + path + " was completed", LIGHT_GREEN);
	setStatusCode(NO_CONTENT);
}

void	Response::_handleCgi()
{
	DEBUG("handleCgi()");
}

/* Perform resource-specific processing on the request payload. */
void	Response::_runPostMethod(std::string& path)
{
	std::ofstream							ofs;

	DEBUG("Post method");
	if (_isMultipartFormRequest()) // && pathIsDirectory(path)) ?
	{
		_handleMultipartContent(path, _request->getBody());
	}
	if (_matchingBlock->cgiDirective()) // && check if cgi in request url
	{
		/* process cgi */
		return (_handleCgi());
	}
	_writeFileContent(path, _request->getBody());
}

/******************************************************************************/
/*                               DELETE METHOD                                */
/******************************************************************************/

/* Remove all current representations of the target resource. */
void	Response::_runDeleteMethod(std::string& path)
{
	int	ret;

	DEBUG("Delete method");
	ret = remove(path.c_str());
	if (ret)
	{
		/* Error case */
		throw (_findErrorCode());
	}
	 /* Successfull case */
	setStatusCode(NO_CONTENT);
	displayMsg(" 🚫 Resource " + path + " was successfully deleted", LIGHT_GREEN);
}

/******************************************************************************/
/*                                  ERROR                                     */
/******************************************************************************/

void	Response::_throwErrorMsg(t_statusCode errorCode, const std::string& message)
{
	std::cerr << RED << "Webserv error: " << message << RESET << std::endl;
	throw(errorCode);
}

void	Response::_throwErrorMsg(const std::string& message)
{
	std::cerr << RED << "Webserv error: " << message << RESET << std::endl;
	throw(INTERNAL_SERVER_ERROR);
}

t_statusCode	Response::_findErrorCode()
{
	if (errno == ENOENT || errno == ENOTDIR)
		return (NOT_FOUND);
	else if (errno == EACCES || errno == EPERM)
		return (FORBIDDEN);
	return (INTERNAL_SERVER_ERROR);
}

/******************************************************************************/
/*                                 HEADERS                                    */
/******************************************************************************/

std::string		Response::_getContentTypeHeader()
{
	size_t			pos;
	std::string		typeExtension;

	/* Check if request is valid */
	if (_statusCode >= 400)
		return (g_mimeType[".html"]);
	/* Check if header is already set */
	if (_headers.find("Content-Type") != _headers.end())
		return (_headers["Content-Type"]);
	pos = _request->getPath().find(".");
	if (pos != std::string::npos)
		typeExtension = _request->getPath().substr(pos);
	return (g_mimeType[typeExtension]);
}

std::string		Response::_getDateHeader()
{
	std::time_t	time;
    char		date[100];

	time = std::time(NULL);
    if (!std::strftime(date, sizeof(date), "%a, %d %b %Y %X GMT", std::localtime(&time)))
	{
		/* Error */
		_throwErrorMsg("strftime() function failed");
	}
	return(std::string(date));
}

/* The keep-alive directive indicates that the client wants the HTTP Connection to persist and remain
open after the current transaction is complete. This is the default setting for HTTP/1.1 requests. */
std::string		Response::_getConnectionHeader()
{
	std::string						connection;
	listOfHeaders::const_iterator	connectionHeader;
	listOfHeaders					requestHeaders;

	if (!_request)
		return ("close");
	connection = "keep-alive";
	requestHeaders = _request->getHeaders();
	connectionHeader = requestHeaders.find("connection");
	if (connectionHeader != requestHeaders.end() && connectionHeader->second == "close")
		connection = "close";
	return (connection);
}

void	Response::_fillExtraHeader()
{
	#ifdef COOKIE
		_fillCookieHeader();
	#endif
}

/******************************************************************************/
/*                                  PATH                                      */
/******************************************************************************/

void	Response::_generateAutoindex(const std::string& path)
{
	Autoindex	autoindex(path);

	_body = autoindex.getIndexPage();
	_headers["Content-type"] = "text/html";
}

bool	Response::_foundIndexPage(DIR* dir, const std::string& indexPage)
{
	struct dirent*	diread;

	while ((diread = readdir(dir)))
	{
		if (diread->d_name == indexPage)
			return (true);
	}
	return (false);
}

bool	Response::_searchOfIndexPage(const listOfStrings& indexes, std::string* path)
{
	listOfStrings::const_iterator	currentIndex;
	DIR*							dir;
	bool							foundIndexPage;

	foundIndexPage = false;
	dir = opendir(path->c_str());
	if (!dir) /* error */
		return false;
	for (currentIndex = indexes.begin(); currentIndex != indexes.end(); currentIndex++)
	{
		if (_foundIndexPage(dir, *currentIndex))
		{
			*path += "/" + *currentIndex;
			foundIndexPage = true;
			break ;
		}
	}
	closedir(dir);
	return (foundIndexPage);
}

/* Directory case */
void	Response::_handleDirectoryPath(std::string* path)
{
	if (*(path->rbegin()) == '/' && _searchOfIndexPage(_matchingBlock->getIndexes(), path))
		return ;
	if (*(path->rbegin()) != '/')
		*path += "/";
	if (_matchingBlock->getAutoindex())
	{
		/* generate autoindex page */
		_generateAutoindex(*path);
	}
}

/* If a request ends with a slash, NGINX treats it as a request for a directory and tries to find an index file in the directory. */
std::string		Response::_buildPath()
{
	std::string		path;
	std::string		uri;

	uri = _request->getPath();
	if (_locationPath != "")
		uri.erase(0, _locationPath.length());
	if (_hasUploadPathDirective())
		path = _matchingBlock->getUploadPath();
	else
		path = _matchingBlock->getRoot(); 
	if (*(path.rbegin()) == '/' && *(uri.begin()) == '/')
		path.erase(path.length() - 1);
	else if (*(path.rbegin()) != '/' && *(uri.begin()) != '/')
		path += "/";
	path += uri;
	if (path[0] == '/')
		path.insert(path.begin(), '.'); // Is it necessary ?
	std::cout << BLUE << "uri: " << _request->getPath() << " | filePath: " << path << RESET << std::endl;
	if (pathIsDirectory(path))
		_handleDirectoryPath(&path);
	return (path);
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

bool	Response::_hasUploadPathDirective()
{
	return ((_method == POST || _method == DELETE) && !_matchingBlock->getUploadPath().empty());
}

bool	Response::_requestIsValid()
{
	return (_request && _request->getStatusCode() < 400);
}

void	Response::_checkBodyLimit()
{
	if (_request->getBodySize() >= _matchingBlock->getClientBodyLimit())
		throw(PAYLOAD_TOO_LARGE);
}

bool	Response::_isMultipartFormRequest()
{
	size_t pos;

	pos = _request->getHeader("content-type").find("multipart/form-data");
	return (pos != std::string::npos);
}

/******************************************************************************/
/*                                  SETTER                                    */
/******************************************************************************/

void	Response::setStatusCode(t_statusCode status)
{
	_statusCode = status;
}

void	Response::setStatusCode(int status)
{
	_statusCode = (t_statusCode)status;
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Block*		Response::getServer() const
{
	return (_server);
}

Block*		Response::getMatchingBlock() const
{
	return (_matchingBlock);
}

Request*	Response::getRequest() const
{
	return (_request);
}

std::string		Response::getResponse() const
{
	return (_response);
}

t_statusCode	Response::getStatusCode() const
{
	return (_statusCode);
}

std::string		Response::getStatusCodeStr() const
{
	return (convertNbToString(_statusCode));
}

t_method	Response::getMethod() const
{
	return (_method);
}

Response::listOfHeaders	Response::getHeaders() const
{
	return (_headers);
}

std::string		Response::getBody() const
{
	return (_body);
}

Response::listOfHttpMethodsFunct	Response::getHttpMethods() const
{
	return (_httpMethods);
}

std::string		Response::getLocationPath() const
{
	return (_locationPath);
}

/******************************************************************************/
/*                                   INIT                                     */
/******************************************************************************/

void	Response::_initHttpMethods()
{
	_httpMethods[GET] = &Response::_runGetMethod;
	_httpMethods[POST] = &Response::_runPostMethod;
	_httpMethods[DELETE] = &Response::_runDeleteMethod;
}

/******************************************************************************/
/*                                  BONUS                                     */
/******************************************************************************/

#ifdef COOKIE
Response::Response(Block *server, Request* request, Cookie& cookies):
	_server(server),
	_request(request),
	_response(""),
	_statusCode(_request->getStatusCode()),
	_method(_request->getMethod()),
	_body(""),
	_locationPath(""),
	_cookies(cookies)
{
	_initHttpMethods();
}

void	Response::_fillCookieHeader()
{
	_response += _cookies.setCookieHeader();
}

Cookie&		Response::getCookies()
{
	return (_cookies);
}

const Cookie&	Response::getCookies() const
{
	return (_cookies);
}

#endif
