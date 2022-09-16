#include "Request.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Request::Request() {}

Request::Request(const std::string& buffer):
	_request(buffer),
	_method(NO_METHOD),
	_uri(""),
	_httpProtocol("")
{

}

Request::Request(const Request &other):
	_request(other.getRequest()),
	_method(other.getMethod()),
	_uri(other.getUri()),
	_httpProtocol(other.getHttpProtocol())
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

	pos = _request.find("\r\n");
	if (pos == std::string::npos)
		return (false);
	std::string requestLine(_request, 0, pos);
	std::cout << "RequestLine : " << requestLine << std::endl;
	parseMethod(requestLine);
	parseUri(requestLine);
	parseHttpProtocol(requestLine);

	std::string headers(_request, pos + 2);
	std::cout << "Headers = '" << headers << "'" << std::endl;
	parseHeaders();
	return (true);
}
	
void	Request::completeRequest(const std::string& buffer)
{
	_request += buffer;
}

std::string		Request::_getNextWord(std::string &line)
{
	size_t	pos;

	pos = line.find(" ");
	std::string word(line, 0, pos);
	line.erase(0, pos + 1);
	std::cout << "word = '" << word << "' | line = '" << line << "'" << std::endl;
	return (word);
}

void	Request::parseMethod(std::string &requestLine)
{
	std::string method = _getNextWord(requestLine);
	std::cout << "method = " << method << std::endl;
}

void	Request::parseUri(std::string &requestLine)
{
	std::string uri = _getNextWord(requestLine);
	std::cout << "uri = '" << uri << "'" << std::endl;
}

void	Request::parseHttpProtocol(std::string &requestLine)
{
	std::string httpProtocol = _getNextWord(requestLine);
	std::cout << "httpProtocol = '" << httpProtocol << "'" << std::endl;
}

void	Request::parseHeaders()
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

