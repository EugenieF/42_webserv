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

void	Request::parseRequest()
{
	std::cout << "Request : " << _request << std::endl;
	parseMethod();
	parseUri();
	parseHttpProtocol();
	parseHeaders();
}
	
void	Request::completeRequest(const std::string& buffer)
{
	_request += buffer;
}

void	Request::parseMethod()
{

}

void	Request::parseUri()
{
	
}

void	Request::parseHttpProtocol()
{
	
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

