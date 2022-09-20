#include "Request.hpp"

/* The HTTP GET request method is used to request a resource from the server.
The GET request should only receive data (the server must not change its state).

The HTTP POST method is used to create or add a resource on the server.
The POST method asks the web server to accept the data contained in the
body of the message. The data type in the HTTP POST body is indicated by the
Content-Type header.

- request line / status line
- HTTP headers [Header: value]
- HTTP body
*/

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Request::Request() {}

Request::Request(Block* server, const std::string& buffer):
	_server(server),
	_request(buffer),
	_method(NO_METHOD),
	_path(""),
	_httpProtocol(""),
	_requestIsValid(true),
	_chunkedTransfer(false)
{
	_initParsingFunct();
}

Request::Request(const Request &other):
	_request(other.getRequest()),
	_method(other.getMethod()),
	_path(other.getPath()),
	_httpProtocol(other.getHttpProtocol()),
	_requestIsValid(other.getRequestValidity()),
	_parsingFunct(other.getParsingFunct()),
	_chunkedTransfer(other.getChunkedTransfer())
{
	*this = other;
}

Request::~Request() {}

Request&	Request::operator=(const Request &other)
{
	if (this != &other)
	{
		_request = other.getRequest();
		_method = other.getMethod();
		_path = other.getPath();
		_httpProtocol = other.getHttpProtocol();
		_requestIsValid = other.getRequestValidity();
		_parsingFunct = other.getParsingFunct();
		_chunkedTransfer = other.getChunkedTransfer();
	}
	return (*this);
}

/******************************************************************************/
/*                                  PARSING                                   */
/******************************************************************************/

bool	Request::parseRequest()
{
	Request::listOfParsingFunctions::const_iterator	ite;

	for (ite = _parsingFunct.begin(); _requestIsValid && ite != _parsingFunct.end(); ite++)
		(this->**ite)();
	printRequestInfo();
	return (true);
}
	
void	Request::completeRequest(const std::string& buffer)
{
	_request += buffer;
}

void	Request::_parseMethod()
{
	std::string method;
	
	_getNextWord(method, " ");
	if (!_server->isMethod(method))
		return (_requestIsInvalid(NOT_IMPLEMENTED));
	if (!_server->isAllowedMethod(method))
		return (_requestIsInvalid(METHOD_NOT_ALLOWED));
	_method = _server->getMethod(method);
}

void	Request::_parsePath()
{
	std::string path;
	
	_getNextWord(path, " ");
	if (path == "" || path[0] != '/')
		return (_requestIsInvalid(BAD_REQUEST));
	if (path.length() > 2048)
		return (_requestIsInvalid(URI_TOO_LONG));
	_path = path;
}

void	Request::_parseHttpProtocol()
{
	std::string httpProtocol;
	
	_getNextWord(httpProtocol, "\r\n");
	if (httpProtocol.find("HTTP") == std::string::npos)
		return (_requestIsInvalid(BAD_REQUEST));
	if (!(httpProtocol == "HTTP/1.0" || httpProtocol == "HTTP/1.1"))
		return (_requestIsInvalid(HTTP_VERSION_NOT_SUPPORTED));
	_httpProtocol = httpProtocol;
}

void	Request::_parseHeaders()
{
	std::string		headerName;
	std::string		headerValue;
	size_t			pos;

	pos = 0;
	while (pos != std::string::npos)
	{
		pos = _getNextWord(headerName, ":");
		_toLowerStr(&headerName); /* Case-insensitive */
		if (pos == std::string::npos)
			break ;
		_getNextWord(headerValue, "\r\n");
		_trimSpaceStr(&headerValue); /* We retrieve spaces around the value */
		std::cout << YELLOW << headerName << ": '" << headerValue << "'" << RESET << std::endl;
		if (headerName.length() > 1000 || headerValue.length() > 4000) // NOT OK, TO SEARCH
			return (_requestIsInvalid(REQUEST_HEADER_FIELDS_TOO_LARGE));
		_headers[headerName] = headerValue;
	}
	_getNextWord(headerName, "\r\n");
}

void	Request::_checkHeaders()
{
	Request::listOfHeaders::const_iterator	ite;
	std::string								contentLength;
	size_t									size;

	if (_headers.find("host") == _headers.end())
		return (_requestIsInvalid(BAD_REQUEST));
	ite = _headers.find("transfer-encoding");
	if (ite != _headers.end() && ite->second.find("chunked") != std::string::npos) // not really sure about this
	{
		_chunkedTransfer = true;
		std::cout << "CHUNKED" << std::endl;	
	}
	else if (_headers.find("content-length") != _headers.end())
	{
		contentLength = _headers["content-length"];
		size = std::strtoul(contentLength.c_str(), NULL, 10);
		std::cout << "size = " << size << std::endl;
		if (contentLength.find_first_not_of("0123456789") != std::string::npos || !size || size >= ULONG_MAX)
			return (_requestIsInvalid(BAD_REQUEST));
		_bodySize = size;
	}
	else
		return (_requestIsInvalid(BAD_REQUEST));
	// _headers.find("Host");
	// _headers.find("Transfer-Encoding");
	// _headers.find("Content-Length");
	// _headers.find("Content-Type");
}

std::string	Request::_trimSpaceStr(std::string *str, const char *toTrim)
{
	str->erase(0, str->find_first_not_of(toTrim));
	str->erase(str->find_last_not_of(toTrim) + 1);
	return (*str);
}

void	Request::_parseBody()
{
	std::string		body;

	std::cout << "request : " << _request << std::endl;
	_getNextWord(body, "\r\n");
	_body = body;
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

std::string		Request::getRequest() const
{
	return (_request);
}

Request::t_method	Request::getMethod() const
{
	return (_method);
}

std::string		Request::getPath() const
{
	return (_path);
}

std::string		Request::getHttpProtocol() const
{
	return (_httpProtocol);
}

Request::listOfParsingFunctions		Request::getParsingFunct() const
{
	return (_parsingFunct);
}

bool	Request::getRequestValidity() const
{
	return (_requestIsValid);
}

t_statusCode	Request::getStatusCode() const
{
	return (_statusCode);
}

std::string		Request::getStatusCodeStr() const
{
	std::stringstream	ss;

	ss << _statusCode;
	return (ss.str());
}

bool	Request::getChunkedTransfer() const
{
	return (_chunkedTransfer);
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

std::string		Request::_toLowerStr(std::string* str)
{
	std::string::iterator ite;

	for (ite = str->begin(); ite != str->end(); ite++)
		*ite = std::tolower(*ite);
	return (*str);
}

size_t	Request::_getNextWord(std::string &word, std::string const& delimiter)
{
	size_t	pos;

	pos = _request.find(delimiter);
	std::string nextWord(_request, 0, pos);
	_request.erase(0, pos + delimiter.length());
	word = nextWord;
	return (pos);
}

void	Request::_requestIsInvalid(t_statusCode code)
{
	_statusCode = code;
	_requestIsValid = false;
}

void	Request::_initParsingFunct()
{
	_parsingFunct.insert(_parsingFunct.end(), &Request::_parseMethod);
	_parsingFunct.insert(_parsingFunct.end(), &Request::_parsePath);
	_parsingFunct.insert(_parsingFunct.end(), &Request::_parseHttpProtocol);
	_parsingFunct.insert(_parsingFunct.end(), &Request::_parseHeaders);
	_parsingFunct.insert(_parsingFunct.end(), &Request::_checkHeaders);
	_parsingFunct.insert(_parsingFunct.end(), &Request::_parseBody);
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

void	Request::printRequestInfo()
{
	Request::listOfHeaders::const_iterator ite;

	std::cout <<  BLUE << "------------ INFO REQUEST -------------" << std::endl;
	std::cout << "          method : " << GREEN << _method << std::endl;
	std::cout << BLUE << "             path : " << GREEN << _path << std::endl;
	std::cout << BLUE << "    httpProtocol : " << GREEN << _httpProtocol << std::endl;
	std::cout << BLUE << "      statusCode : " << GREEN << _statusCode << std::endl;
	for (ite = _headers.begin(); ite != _headers.end(); ite++)
	{
		for (int i = ite->first.length(); i < 16 ; i++)
			std::cout << " ";
		std::cout << BLUE << ite->first << " : '" << GREEN << ite->second << BLUE << "'" << std::endl;
	}
	std::cout << BLUE << "            body : '" << GREEN << _body << BLUE << "'" << std::endl;
	std::cout << "---------------------------------------" << std::endl;
	std::cout << RESET << std::endl;
}
