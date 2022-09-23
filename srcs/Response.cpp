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
	_matchingBlock = _server;
}

void	Response::_processMethod()
{
	Response::listOfHttpMethods::const_iterator	ite;

	// /* Check method validity */
	if (!_matchingBlock->isAllowedMethod(_method))
		return (setStatusCode(METHOD_NOT_ALLOWED));
	// _buildPath();
	ite = _httpMethods.find(_method);
	if (ite == _httpMethods.end())
			return (setStatusCode(METHOD_NOT_ALLOWED));
	(this->*ite->second)();
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
	_generateResponseLine();
	_generateHeaders();
	_response += _body;
}

void	Response::_generateResponseLine()
{
	_response = "HTTP/1.1 "
		+ getStatusCodeStr() + " "
		+ g_statusCode[_statusCode]
		+ "\r\n";
}

void	Response::_generateHeaders()
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
	std::string	statusPage;

	statusPage = "<!DOCTYPE html>\n\
	  	<html><head>\n\
	  	<title>" + getStatusCodeStr() + " - " + g_statusCode[_statusCode] + "</title>\n\
	  	</head>\n\
	  	<body><p>Hello world!</p></body>\n\
	  	</html>";
	return (statusPage);
}

/******************************************************************************/
/*                                 METHODS                                    */
/******************************************************************************/

void	Response::_getMethod()
{
	std::cout << GREEN << "GET METHOD" << RESET << std::endl;
}

void	Response::_postMethod()
{
	std::cout << GREEN << "POST METHOD" << RESET << std::endl;
}

void	Response::_deleteMethod()
{
	std::cout << GREEN << "DELETE METHOD" << RESET << std::endl;
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

std::string		Response::_buildPath()
{
	std::string		path;

	if (_uploadPathDirective())
		path = _matchingBlock->getUploadPath();
	else
		path = _matchingBlock->getRoot();
	path += "/" + _request->getPath();
	return (path);
}

bool	Response::_uploadPathDirective()
{
	return ((_method == POST || _method == DELETE) && !_matchingBlock->getUploadPath().empty());
}

std::string		Response::_getContentTypeHeader()
{
	if (!_requestIsValid())
		return (g_mimeType[".html"]);
	return (g_mimeType[".txt"]);
}

std::string		Response::_getDateHeader()
{
	std::time_t	time;
    char		mbstr[100];

	time = std::time(NULL);
    if (!std::strftime(mbstr, sizeof(mbstr), "%a, %d %b %Y %X GMT", std::localtime(&time)))
	{ /* Error Date */ }
	return (std::string(mbstr));
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
