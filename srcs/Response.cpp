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
	_method(_request->getMethod())
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

void	Response::_selectMatchingBlock()
{
	_matchingBlock = _server->getMatchingBlock(_request->getHost(), _request->getPath());
}

void	Response::_processMethod()
{
	Response::listOfHttpMethodsFunct::const_iterator	ite;
	std::string											path;

	// /* Check method validity */
	if (!_matchingBlock->isAllowedMethod(_method))
		return (setStatusCode(METHOD_NOT_ALLOWED));
	path = _buildPath();
	if (path.empty())
		return (setStatusCode(NOT_FOUND));
	ite = _httpMethods.find(_method);
	if (ite == _httpMethods.end())
			return (setStatusCode(METHOD_NOT_ALLOWED));
	(this->*ite->second)(path);
	_body = "That's the response body for testing";
}

void	Response::generateResponse()
{
	if (_requestIsValid()) /* Handle valid request */
	{
		_selectMatchingBlock();
		if (!_checkBodyLimit())
			return (setStatusCode(PAYLOAD_TOO_LARGE));
		_processMethod();
	}
	else /*	Handle invalid request */
	{
		_body = _generateErrorPage();
	}
	_fillResponseLine();
	_fillHeaders();
	_response += _body;
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
	for (ite = _headers.begin(); ite != _headers.end(); ite++)
		_response += ite->first + ": " + ite->second + "\r\n";
}

std::string		Response::_generateErrorPage()
{
	std::string	errorPage;

	errorPage = _server->getErrorPage(_statusCode);
	if (errorPage.empty())
	{
		errorPage = "<!DOCTYPE html>\n\
	  		<html><head>\n\
	  		<title>" + getStatusCodeStr() + " - " + g_statusCode[_statusCode] + "</title>\n\
	  		</head>\n\
	  		<body><p>Hello world!</p></body>\n\
	  		</html>";
	}
	return (errorPage);
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
	if (_pathIsFile(path))
		return (_readFileContent(path));
	if (_pathIsDirectory(path))
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

	if (_pathIsAccessible(path))
	{
		/* File already exist */
		return(setStatusCode(CONFLICT));
	}
	file.open(path.c_str(), std::ofstream::out);
	if (!file.is_open())
	{
		/* error */
		// setStatusCode();
	}
	file << _request->getBody();
	if (file.bad())
	{
		/* error */
		// setStatusCode();
	}
	file.close();
	setStatusCode(CREATED);
}

void	Response::_handleUploadFile()
{
	std::cout << GREEN << "handleUploadFile()" << RESET << std::endl;
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
	if (_matchingBlock->uploadPathDirective())
	{
		/* process upload --> create_file ? */
	}
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
		return (_setErrorCodeWithErrno());
	setStatusCode(NO_CONTENT); /* Successfull case */

}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

void	Response::_setErrorCodeWithErrno()
{
	if (errno == ENOENT || errno == ENOTDIR)
		return (setStatusCode(NOT_FOUND));
	else if (errno == EACCES || errno == EPERM)
		return (setStatusCode(FORBIDDEN));
	setStatusCode(INTERNAL_SERVER_ERROR);
}

std::string		Response::_buildPath()
{
	std::string		path;

	if (_hasUploadPathDirective())
		path = _matchingBlock->getUploadPath();
	else
		path = _matchingBlock->getRoot();
	if (_request->getPath()[0] != '/')
		path += "/";
	path += _request->getPath();
	std::cout << BLUE << "PATH = " << path << RESET << std::endl;
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
	if (!_requestIsValid())
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
    char		mbstr[100];

	time = std::time(NULL);
    if (!std::strftime(mbstr, sizeof(mbstr), "%a, %d %b %Y %X GMT", std::localtime(&time)))
	{ /* Error Date */ }
	return(std::string(mbstr));
}

bool	Response::_requestIsValid()
{
	return (_request && _request->getStatusCode() < 400);
}

bool	Response::_checkBodyLimit()
{
	return (_request->getBodySize() < _matchingBlock->getClientBodyLimit());
}

void	Response::setStatusCode(t_statusCode status)
{
	_statusCode = status;
}

void	Response::setStatusCode(int status)
{
	_statusCode = (t_statusCode)status;
}

bool	Response::_pathIsFile(const std::string& path)
{
	struct stat s;

	return (stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFREG));
}

bool	Response::_pathIsDirectory(const std::string& path)
{
	struct stat s;

	return (stat(path.c_str(), &s) == 0 && (s.st_mode & S_IFDIR));
}

bool	Response::_pathIsAccessible(const std::string& path)
{
	int	ret;

	ret = access(path.c_str(), F_OK); 
	return (!ret);
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
