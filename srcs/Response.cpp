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
/*                                 METHODS                                    */
/******************************************************************************/

bool	Response::_fileExists(const std::string& path)
{
	int	ret;

	ret = access(path.c_str(), F_OK); 
	return (!ret);
}

bool	Response::_isDirectory(const std::string& path)
{
	struct stat s;

	if( stat(path.c_str(), &s) == 0 )
	{
    	if(s.st_mode & S_IFDIR) 
    	{
			return (true);
        //it's a directory
    	}
    	else if(s.st_mode & S_IFREG)
    	{
        	//it's a file
			return (false);
    	}
    	else
    	{
        //something else
			return (false);
    	}
	}
	return (false);

}

void	Response::_checkFilePath(std::string& path)
{
	(void)path;
	// If it's a directory {
		// check index files -> get_index
		// check Autoindex ??
}

/*  Transfer a current representation of the target resource. */
void	Response::_getMethod(std::string& path)
{
	std::ifstream	ifs;

	(void)path;
	std::cout << GREEN << "GET METHOD" << RESET << std::endl;
	// Do redirection if necessary
	_checkFilePath(path);
	// _body = get_file_content
	setStatusCode(OK);
}

/* Perform resource-specific processing on the request payload. */
void	Response::_postMethod(std::string& path)
{
	std::ofstream	ofs;

	(void)path;
	std::cout << GREEN << "POST METHOD" << RESET << std::endl;
	// cgi_path ?
	// handle_upload -> create_file
}

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

void	Response::_handleUploadFile()
{
	std::cout << GREEN << "handleUploadFile()" << RESET << std::endl;
}

void	Response::_handleCgi()
{
	std::cout << GREEN << "handleCgi()" << RESET << std::endl;
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

	if (!_requestIsValid())
		return (g_mimeType[".html"]);
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
