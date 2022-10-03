#include "Response.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Response::Response(Block *server, Request* request):
	_server(server),
	_request(request),
	_response(""),
	_statusCode(_request->getStatusCode()),
	_body(""),
	_method(_request->getMethod()),
	_locationPath("")
{
	_initHttpMethods();
}

Response::Response(const Response &other):
	_request(other.getRequest()),
	_response(other.getResponse()),
	_statusCode(other.getStatusCode()),
	_body(other.getBody())
{
	*this = other;
}

Response::~Response() {}

Response&	Response::operator=(const Response &other)
{
	if (this != &other)
	{
		_request = other.getRequest();
		_response = other.getResponse();
		_statusCode = other.getStatusCode();
		_body = other.getBody();
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

	// /* Check method validity */
	if (!_matchingBlock->isAllowedMethod(_method))
		throw(METHOD_NOT_ALLOWED);
	path = _buildPath();
	if (path.empty())
		throw(NOT_FOUND);
	ite = _httpMethods.find(_method);
	if (ite == _httpMethods.end())
			throw(METHOD_NOT_ALLOWED);
	(this->*ite->second)(path);
}

void	Response::generateResponse()
{
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
		}
	}
	else /*	Handle invalid request */
	{
		_body = _generateErrorPage();
	}
	_fillResponseLine();
	_fillHeaders();
	_response += _body + "\r\n";
}

std::string		Response::_generateErrorPage()
{
	std::string	errorPage;

	errorPage = _matchingBlock->getErrorPage(_statusCode);
	if (errorPage.empty())
	{
		errorPage = "<!DOCTYPE html>\n\
	  		<html><head>\n\
	  		<title>" + getStatusCodeStr() + " - " + g_statusCode[_statusCode] + "</title>\n\
	  		</head>\n\
	  		<body><p>- Error Page -</p></body>\n\
	  		</html>";
	}
	return (errorPage);
}

/******************************************************************************/
/*                              FILL RESPONSE                                   */
/******************************************************************************/

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
	for (ite = _headers.begin(); ite != _headers.end(); ite++)
		_response += ite->first + ": " + ite->second + "\r\n";
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

bool	Response::_foundIndexPage(DIR* dir, std::string indexPage)
{
	struct dirent*	diread;

	while ((diread = readdir(dir)))
	{
		if (diread->d_name == indexPage)
			return (true);
	}
	return (false);
}

bool	Response::_searchOfIndexPage(listOfStrings indexes, std::string* path)
{
	listOfStrings::iterator	currentIndex;
	DIR*					dir;
	bool					foundIndexPage;

	foundIndexPage = false;
	dir = opendir(path->c_str());
	if (dir) /* error */
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

void	Response::_readFileContent(std::string& path)
{
	std::ifstream		file;
	std::stringstream	fileContent;
	
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

void	Response::_generateAutoindex(std::string& path)
{
	Autoindex	autoindex(path);

	_body = autoindex.getIndexPage();
	_headers["Content-type"] = "text/html";
}

/*  GET method : "Transfer a current representation of the target resource." */
void	Response::_getMethod(std::string& path)
{
	std::string		filePath;

	std::cout << GREEN << "GET METHOD" << RESET << std::endl;
	if (_matchingBlock->redirectDirective())
	{
		/* Do redirection */
		return (_handleRedirection());
	}
	if (pathIsFile(path))
		return (_readFileContent(path));
	if (pathIsDirectory(path))
	{
		/* Directory case */
		if (*(path.rbegin()) == '/' && _searchOfIndexPage(_matchingBlock->getIndexes(), &path))
			return (_readFileContent(path));
		if (*(path.rbegin()) != '/')
			path += "/";
		if (_matchingBlock->getAutoindex())
		{
			/* generate autoindex page */
			return (_generateAutoindex(path));
		}
	}
	setStatusCode(NOT_FOUND);
}

/******************************************************************************/
/*                               POST METHOD                                  */
/******************************************************************************/

void	Response::_writeFileContent(std::string& path)
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
		return (_throwErrorMsg("Can't open file '" + path + "'"));
	}
	file << _request->getBody();
	if (file.bad())
	{
		/* error */
		return (_throwErrorMsg("An error occurred while writing '" + path + "'"));
	}
	file.close();
	// setStatusCode(NO_CONTENT); ?? 
}

void	Response::_handleCgi()
{
	std::cout << GREEN << "handleCgi()" << RESET << std::endl;
}

/* Perform resource-specific processing on the request payload. */
void	Response::_postMethod(std::string& path)
{
	std::ofstream	ofs;

	(void)path;
	std::cout << GREEN << "POST METHOD" << RESET << std::endl;
	if (_matchingBlock->cgiDirective())
	{
		/* process cgi */
	}
	// if (_matchingBlock->uploadPathDirective()) ??
	_request->getHeader("content-type");
	_writeFileContent(path);
}

/******************************************************************************/
/*                               DELETE METHOD                                */
/******************************************************************************/

/* Remove all current representations of the target resource. */
void	Response::_deleteMethod(std::string& path)
{
	int	ret;

	std::cout << GREEN << "DELETE METHOD" << RESET << std::endl;
	ret = remove(path.c_str());
	if (ret) /* Error case */
	{
		_throwErrorMsg(_getErrorCodeWithErrno(), "Can't remove '" + path + "' in DELETE method");
	}
	setStatusCode(NO_CONTENT); /* Successfull case */

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

t_statusCode	Response::_getErrorCodeWithErrno()
{
	if (errno == ENOENT || errno == ENOTDIR)
		return (NOT_FOUND);
	else if (errno == EACCES || errno == EPERM)
		return (FORBIDDEN);
	return (INTERNAL_SERVER_ERROR);
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

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
	if (_request->getPath()[0] != '/')
		path += "/";
	path += uri;
	if (path[0] == '/')
		path.insert(path.begin(), '.');
	std::cout << BLUE << "*** PATH = " << path << " ***" << RESET << std::endl;
	return (path);
}

bool	Response::_hasUploadPathDirective()
{
	return ((_method == POST || _method == DELETE) && !_matchingBlock->getUploadPath().empty());
}

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

bool	Response::_requestIsValid()
{
	return (_request && _request->getStatusCode() < 400);
}

void	Response::_checkBodyLimit()
{
	if (_request->getBodySize() >= _matchingBlock->getClientBodyLimit())
		throw(PAYLOAD_TOO_LARGE);
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

std::string		Response::getBody() const
{
	return (_body);
}

/******************************************************************************/
/*                                   INIT                                     */
/******************************************************************************/

void	Response::_initHttpMethods()
{
	_httpMethods[GET] = &Response::_getMethod;
	_httpMethods[POST] = &Response::_postMethod;
	_httpMethods[DELETE] = &Response::_deleteMethod;
}
