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

Request::Request():
	_request(""),
	_requestStatus(INCOMPLETE_REQUEST),
	_statusCode(OK),
	_method(NO_METHOD),
	_path(""),
	_httpProtocol(""),
	_bodySize(0),
	_body(""),
	_chunkedTransfer(false),
	_host(""),
	_port(UNDEFINED_PORT)
{
	_initParsingFunct();
}

Request::Request(const std::string& buffer):
	_request(buffer),
	_requestStatus(INCOMPLETE_REQUEST),
	_statusCode(OK),
	_method(NO_METHOD),
	_path(""),
	_httpProtocol(""),
	_bodySize(0),
	_body(""),
	_chunkedTransfer(false),
	_host(""),
	_port(UNDEFINED_PORT)
{
	_initParsingFunct();
}

Request::Request(const Request &other)
{
	*this = other;
}

Request::~Request() {}

Request&	Request::operator=(const Request &other)
{
	if (this != &other)
	{
		_request = other.getRequest();
		_requestStatus = other.getRequestStatus();
		_statusCode = other.getStatusCode(),
		_method = other.getMethod();
		_path = other.getPath();
		_httpProtocol = other.getHttpProtocol();
		_headers = other.getHeaders();
		_bodySize = other.getBodySize();
		_body = other.getBody();
		_parsingFunct = other.getParsingFunct();
		_chunkedTransfer = other.getChunkedTransfer();
		_host = other.getHost();
		_port = other.getPort();
	}
	return (*this);
}

/******************************************************************************/
/*                                  PARSING                                   */
/******************************************************************************/

t_requestStatus	Request::parseRequest()
{
	Request::listOfParsingFunctions::const_iterator	ite;

	if (_chunkedTransfer && _requestStatus != COMPLETE_REQUEST)
		_parseChunks();
	else
	{
		for (ite = _parsingFunct.begin();
			_requestStatus != COMPLETE_REQUEST && ite != _parsingFunct.end(); ite++)
			(this->**ite)();
	}
	printRequestInfo();
	return (_requestStatus);
}
	
void	Request::completeRequest(const std::string& buffer)
{
	_request += buffer;
}

void	Request::_parseMethod()
{
	std::string method;

	_getNextWord(method, " ");
	if (g_httpMethod.isHttpMethod(method) == false)
		return (_requestIsInvalid(NOT_IMPLEMENTED));
	_method = g_httpMethod.getMethod(method);
}

void	Request::_parsePath()
{
	std::string path;
	
	_getNextWord(path, " ");
	if (path == "" || path[0] != '/')
		return (_requestIsInvalid(BAD_REQUEST));
	if (path.length() > 2048)
		return (_requestIsInvalid(URI_TOO_LONG));
	/*  Handle query "?" in URI  */
	_path = path;
}

void	Request::_parseHttpProtocol()
{
	std::string httpProtocol;
	
	_getNextWord(httpProtocol, "\r\n");
	if (httpProtocol.find("HTTP") == std::string::npos)
		return (_requestIsInvalid(BAD_REQUEST));
	if (httpProtocol != "HTTP/1.1")
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
		if (headerName.length() > 1000 || headerValue.length() > 4000) // NOT OK, TO SEARCH
			return (_requestIsInvalid(REQUEST_HEADER_FIELDS_TOO_LARGE));
		_headers[headerName] = headerValue;
	}
	_getNextWord(headerName, "\r\n");
}

bool	Request::_parseHostHeader()
{
	Request::listOfHeaders::const_iterator	ite;
	size_t									pos;

	ite = _headers.find("host");
	if (ite == _headers.end())
		return (false);
	_host = ite->second;
	pos = _host.find(":");
	if (pos != std::string::npos)
		_host = _host.substr(0, pos);
	if (pos + 1 != std::string::npos)
		_port = atoi(ite->second.substr(pos + 1).c_str());
	return (true);
}

void	Request::_checkHeaders()
{
	Request::listOfHeaders::const_iterator	ite;
	std::string								contentLength;
	size_t									size;

	if (!_parseHostHeader())
		return (_requestIsInvalid(BAD_REQUEST));
	if (_method != POST)
		return ;
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
	// _headers.find("Content-Type");
}

void	Request::_parseBody()
{
	std::string		body;

	if (_requestStatus == COMPLETE_REQUEST)
		return ;
	if (_chunkedTransfer)
		_parseChunks();
	else
	{
		_getNextWord(_body, "\r\n\r\n");
		_bodySize = _body.length();
		return (_setRequestStatus(COMPLETE_REQUEST));
	}
	// if (_request.length() < _bodySize) {}
}

/* DECODING CHUNKED pseudo-code :

     length := 0
     read chunk-size, chunk-ext (if any), and CRLF
     while (chunk-size > 0) {
        read chunk-data and CRLF
        append chunk-data to decoded-body
        length := length + chunk-size
        read chunk-size, chunk-ext (if any), and CRLF
     }
     read trailer field
     while (trailer field is not empty) {
        if (trailer field is allowed to be sent in a trailer) {
            append trailer field to existing header fields
        }
        read trailer-field
     }
     Content-Length := length
     Remove "chunked" from Transfer-Encoding
     Remove Trailer from existing header fields
*/

void	Request::_parseChunks()
{
	long			chunkSize;
	std::string		chunk;
	size_t			pos;

	if (!_reachedEndOfChunkedBody())
	{
		_bodySize = _request.length(); // Not sure about this...
		std::cout << RED << "*****  CHUNKS, INCOMPLETE REQUEST  *****" << std::endl;
		std::cout << "request = " << _request << RESET << std::endl;
		return (_setRequestStatus(INCOMPLETE_REQUEST));
	}
	while (1)
	{
		std::cout << GREEN << "***** COMPLETE CHUNKED  *****" << RESET << std::endl;
		chunkSize = 0;
		pos = _getNextWord(chunk, "\r\n");
		if (pos == std::string::npos)
			return (_requestIsInvalid(BAD_REQUEST));
		chunkSize = std::strtol(chunk.c_str(), NULL, 16);
		if (chunk.find_first_not_of("0123456789abcdefABCDEF") != std::string::npos)
			return (_requestIsInvalid(BAD_REQUEST));
		if (!chunkSize)
			return (_setRequestStatus(COMPLETE_REQUEST));
		chunk = _getNextWord(chunkSize);
		std::cout << RED << "chunkSize : " << chunkSize << " | chunk = '" << chunk << "'" << RESET << std::endl;
		_body += chunk;
	}
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

std::string		Request::getRequest() const
{
	return (_request);
}

t_requestStatus		Request::getRequestStatus() const
{
	return (_requestStatus);
}

t_statusCode	Request::getStatusCode() const
{
	return (_statusCode);
}

std::string		Request::getStatusCodeStr() const
{
	return (convertNbToString(_statusCode));
}

t_method	Request::getMethod() const
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

Request::listOfHeaders	Request::getHeaders() const
{
	return (_headers);
}

std::string		Request::getHeader(const std::string& headerName)
{
	if (_headers.find(headerName) != _headers.end())
		return (_headers[headerName]);
	return ("");
}

size_t	Request::getBodySize() const
{
	return (_bodySize);
}

std::string		Request::getBody() const
{
	return (_body);
}

Request::listOfParsingFunctions		Request::getParsingFunct() const
{
	return (_parsingFunct);
}

bool	Request::getChunkedTransfer() const
{
	return (_chunkedTransfer);
}

std::string		Request::getHost() const
{
	return (_host);
}

int		Request::getPort() const
{
	return (_port);
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

bool	Request::_reachedEndOfChunkedBody()
{
	return (_request.find("0\r\n\r\n") != std::string::npos);
}

void	Request::_setRequestStatus(t_requestStatus status)
{
	_requestStatus = status;
}

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

std::string		Request::_getNextWord(size_t sizeWord)
{
	std::string nextWord;

	nextWord = _request.substr(0, sizeWord);
	_request.erase(0, sizeWord + 2);
	return (nextWord);
}

void	Request::_requestIsInvalid(t_statusCode code)
{
	_statusCode = code;
	_requestStatus = INVALID_REQUEST;
}

std::string	Request::_trimSpaceStr(std::string *str, const char *toTrim)
{
	str->erase(0, str->find_first_not_of(toTrim));
	str->erase(str->find_last_not_of(toTrim) + 1);
	return (*str);
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
/*                                 DISPLAY                                    */
/******************************************************************************/

void	Request::printRequestInfo()
{
	Request::listOfHeaders::const_iterator ite;

	std::cout <<  BLUE << "------------ INFO REQUEST -------------" << std::endl;
	std::cout << "          method : " << GREEN << _method << std::endl;
	std::cout << BLUE << "            path : " << GREEN << _path << std::endl;
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
