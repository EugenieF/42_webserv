#include "Request.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Request::Request() {}

Request::Request(Block* server, const std::string& buffer):
	_server(server),
	_request(buffer),
	_method(NO_METHOD),
	_uri(""),
	_httpProtocol(""),
	_requestIsValid(true)
{
	_initParsingFunct();
}

Request::Request(const Request &other):
	_request(other.getRequest()),
	_method(other.getMethod()),
	_uri(other.getUri()),
	_httpProtocol(other.getHttpProtocol()),
	_requestIsValid(true)
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
		_uri = other.getUri();
		_httpProtocol = other.getHttpProtocol();
	}
	return (*this);
}

/******************************************************************************/
/*                                  PARSING                                   */
/******************************************************************************/

bool	Request::parseRequest()
{
	size_t	pos;
	// Request::listOfParsingFunctions::const_iterator	ite;

	pos = _request.find("\r\n");
	if (pos == std::string::npos)
		return (false);
	std::string requestLine(_request, 0, pos);
	std::cout << "RequestLine : " << requestLine << std::endl;

	// for (ite = _parsingFunct.begin(); ite != _parsingFunct.end(); ite++)
	// {
	// 	*ite();
	// }
	_parseMethod(requestLine);
	_parseUri(requestLine);
	_parseHttpProtocol(requestLine);

	std::string headers(_request, pos + 2);
	std::cout << "Headers = '" << headers << "'" << std::endl;
	_parseHeaders();
	return (true);
}
	
void	Request::completeRequest(const std::string& buffer)
{
	_request += buffer;
}

void	Request::_parseMethod(std::string &requestLine)
{
	std::string method = _getNextWord(requestLine);
	std::cout << "method = " << method << std::endl;
	if (_server->isAllowedMethod(method))
		std::cout << "METHOD IS OK" << std::endl;
		// _method = method;
}

void	Request::_parseUri(std::string &requestLine)
{
	std::string uri = _getNextWord(requestLine);
	std::cout << "uri = '" << uri << "'" << std::endl;
	_uri = uri;
}

void	Request::_parseHttpProtocol(std::string &requestLine)
{
	std::string httpProtocol = _getNextWord(requestLine);
	std::cout << "httpProtocol = '" << httpProtocol << "'" << std::endl;
	if (!(httpProtocol == "HTTP/1.0" || httpProtocol == "HTTP/1.1"))
	{
		std::cout << "ERROR Http protocol" << std::endl;
		return (_requestIsInvalid(HTTP_VERSION_NOT_SUPPORTED));
	}
	_httpProtocol = httpProtocol;
}

void	Request::_parseHeaders()
{
	
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

std::string		Request::getRequest() const
{
	return (_request);
}

t_method	Request::getMethod() const
{
	return (_method);
}

std::string		Request::getUri() const
{
	return (_uri);
}

std::string		Request::getHttpProtocol() const
{
	return (_httpProtocol);
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

std::string		Request::_getNextWord(std::string &line)
{
	size_t	pos;

	pos = line.find(" ");
	std::string word(line, 0, pos);
	line.erase(0, pos + 1);
	std::cout << "word = '" << word << "' | line = '" << line << "'" << std::endl;
	return (word);
}

void	Request::_requestIsInvalid(t_statusCode code)
{
	_statusCode = code;
	_requestIsValid = false;
}

void	Request::_initParsingFunct()
{
	// _parsingFunct = {&Request::_parseMethod, &Request::_parseUri, &Request::_parseHttpProtocol};
	// _parsingFunct.insert(&Request::_parseMethod);
	// _parsingFunct[1] = &Request::_parseUri;
	// _parsingFunct[2] = &Request::_parseHttpProtocol;
}
