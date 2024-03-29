# include "debug.hpp"
# include "Response.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Response::Response(Block *server, Request* request, Env& env, Session* session):
	_server(server),
	_request(request),
	_response(""),
	_statusCode(_request->getStatusCode()),
	_method(_request->getMethod()),
	_body(""),
	_locationPath(""),
	_fd(request->getFd()),
	_env(&env),
	_is_cgi(false),
	_cgipath(""),
	_session(session)
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
		_fd = other.getFd();
		_env = other.getEnv();
		_cgipath = other.getCgiProgram();
		_session = other.getSession();
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
	//DEBUG("BUILD PATH = " + _builtPath);
	if (path.empty())
		throw(NOT_FOUND);
	/* Find corresponding http method */
	ite = _httpMethods.find(_method);
	if (ite == _httpMethods.end())
		throw(METHOD_NOT_ALLOWED);
	if (_matchingBlock->redirectDirective())
	{
		/* Do redirection */
		return (_handleRedirection());
	}
	(this->*ite->second)();
}

void	Response::generateResponse()
{
	/* Generate CGI here */
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
	if (!_is_cgi)
		_fillHeaders();
	_response += _body + "\r\n";
}

/******************************************************************************/
/*                              FILL RESPONSE                                   */
/******************************************************************************/

void	Response::_fillErrorBody()
{
	std::string		errorFile;

	try
	{
		errorFile = _matchingBlock->getErrorPage(_statusCode);
		_readFileContent(errorFile);
	}
	catch(const t_statusCode& errorCode)
	{
		_body = _generateErrorPage();
	}
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
	_headers["Date"] = getFormattedDate();
	_headers["Connection"] = _getConnectionHeader();
	for (ite = _headers.begin(); ite != _headers.end(); ite++)
		_response += ite->first + ": " + ite->second + "\r\n";
	_fillCookieHeader();
	/* An empty line is placed after the series of HTTP headers,
	to divide the headers from the message body */
	_response += "\r\n";
}

/******************************************************************************/
/*                               GET METHOD                                   */
/******************************************************************************/

void   Response::_parseQuery()
{
	std::string     query;
	std::string     name;
	std::string     content;
	size_t          pos;

	pos = 0;
	query = _request->getQuery();
	while (pos < query.size())
	{
		name = query.substr(pos, query.find("="));
		query.erase(pos, name.length() + 1);
		content = query.substr(pos, query.find("&"));
		query.erase(pos, content.length() + 1);
		if (name == "color" && content.size() > 7)
			content = "#" + content.substr(content.size() - 6);
		_session->completePurchase(name, content, &_msg);
	}
}

void	Response::_handleRedirection()
{
	setStatusCode(_matchingBlock->getRedirectCode());
	_headers["Location"] = _matchingBlock->getRedirectUri();
}

void	Response::_readFileContent(const std::string& path)
{
	std::ifstream		file;
	std::stringstream	fileContent;

	/* Check if file is accessible */
	if (!pathIsAccessible(path))
		throw(NOT_FOUND);
	file.open(path.c_str(), std::ifstream::in);
	/* Check if file was successfully opened */
	if (!file.is_open())
	{
		/* An error occured */
		_throwErrorMsg("Can't open file '" + path + "'");
	}
	/* We read the file */
	fileContent << file.rdbuf();
	_body = fileContent.str();
	file.close();
}

/*  GET method : "Transfer a current representation of the target resource." */
void	Response::_runGetMethod()
{
	//DEBUG("Get method");
	_is_cgi = _isCgi(_builtPath);
	if (_is_cgi)
	{
		/* process cgi */
		return (_handleCgi());
	}
	if (_request->getPath() == "/form_order")
	{
		_body = _generateFormOrderPage();
		_headers["Content-Type"] = g_mimeType[".html"];
	}
	else if (_request->getPath() == "/form_gallery")
	{
		_body = _generateGalleryPage();
		_headers["Content-Type"] = g_mimeType[".html"];
	}
	else if (_body.empty()) /* If there is no autoindex */
	{
		if (_request->getPath() == "/form_accept.html" && !_request->getQuery().empty())
		{
			_parseQuery();
		}
		_readFileContent(_builtPath);
	}
}

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
	{
		throw(BAD_REQUEST);
	}
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
	{
		throw(BAD_REQUEST);
	}
	boundary = "--" + boundary;
	return (boundary);
}

size_t	Response::_getField(
	std::string contentDisposition, const std::string& field, std::string* name)
{
	size_t	pos;

	pos = contentDisposition.find(field);
	if (pos == std::string::npos)
	{
		*name = "";
		return (pos);
	}
	pos += field.size();
	contentDisposition.erase(0, pos);
	*name = contentDisposition.substr(0, contentDisposition.find("\""));
	return (pos);
}

void	Response::_parseContent(std::string body, const std::string& boundary)
{
	size_t		pos;
	std::string	contentDisposition;
	std::string	filename;
	std::string	name;
	std::string	content;

	pos = body.find("Content-Disposition:");
	if (pos == std::string::npos)
		throw(BAD_REQUEST);
	contentDisposition = body.substr(pos, body.find("\r\n"));
	_getField(contentDisposition, "name=\"", &name);
	_getField(contentDisposition, "filename=\"", &filename);
	body.erase(0, body.find("\r\n\r\n") + 4);
	content = body.substr(0, body.find("\r\n" + boundary));
	if (filename != "")
	{
		/* filenamePath = _uploadPath + filename */
		if (_uploadPath[_uploadPath.length() - 1] != '/' && filename[0] != '/')
			_uploadPath += "/";
		_writeFileContent(_uploadPath + filename, content);
	}
	else
	{
		content = body.substr(0, body.find("\r\n" + boundary));
		_session->completePurchase(name, content, &_msg);
	}
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

void	Response::_handleMultipartContentCgi(std::string body)
{
	std::string	boundary;
	std::string	filename;
	std::string	contentDisposition;
	size_t		pos;
	size_t		posFilename;

	posFilename = 0;
	boundary = _getBoundary(_request->getHeader("content-type"));
	while (body.find(boundary + "\r\n") != std::string::npos)
	{
		posFilename += body.find(boundary + "\r\n") + boundary.length() + 2;
		body.erase(0, body.find(boundary + "\r\n") + boundary.length() + 2);
		pos = body.find("Content-Disposition:");
		if (pos == std::string::npos)
			throw(BAD_REQUEST);
		contentDisposition = body.substr(pos, body.find("\r\n"));
		posFilename += _getField(contentDisposition, "filename=\"", &filename);
		if (filename != "")
		{
			if (_uploadPath[_uploadPath.length() - 1] != '/' && filename[0] != '/')
				_uploadPath += "/";
			_request->insertUploadPath(posFilename, _uploadPath);
			_cgiFilenames.insert(_cgiFilenames.begin(), _uploadPath + filename);
			_msg += "   ✅📄 File '" + _uploadPath + filename
				+ "' was successfully uploaded with CGI";
		}
	}
}

void	Response::_handleMultipartContent(std::string body)
{
	std::string	boundary;

	boundary = _getBoundary(_request->getHeader("content-type"));
	while (body.find(boundary + "\r\n") != std::string::npos)
	{
		body.erase(0, body.find(boundary + "\r\n") + boundary.length() + 2);
		_parseContent(body, boundary);
	}
	if (_request->getPath() == "/form_accept")
	{
		_body = _generateFormAcceptPage();
		_headers["Content-Type"] = g_mimeType[".html"];
	}
	else if (_request->getPath() == "/form_upload")
	{
		_body = "OK";
	}
}

/******************************************************************************/
/*                               POST METHOD                                  */
/******************************************************************************/

void	Response::_writeFileContent(const std::string& path, const std::string& content)
{
	std::ofstream	file;
	std::string		pathDir;
	std::string		filename;
	std::string		new_file;
	size_t			separator;

	separator = path.rfind("/");
	pathDir = path.substr(0, separator + 1);
	/* Check if directory exists */
	if (!pathIsAccessible(pathDir))
		throw (BAD_REQUEST);
	filename = path.substr(separator + 1);
	if (filename.empty()) { /* Case: no input file (TODO) */
		throw (BAD_REQUEST);
	} else if (pathIsAccessible(path)) { /* Case: file already exists */
		new_file = generateCopyFilename(pathDir, filename);
	} else { /* Case: file doesn't exist */
		new_file = path;
	}
	setStatusCode(CREATED);
	file.open(new_file.c_str(), std::ios::out | std::ios::binary);
	/* Check if file was successfully opened */
	if (!file.is_open())
	{
		/* An error occured */
		_throwErrorMsg("Can't open file '" + new_file + "'");
	}
	/* We write in file */
	file << content;
	/* Check if writing was successfully performed */
	if (file.bad())
	{
		/* An error occured */
		_throwErrorMsg("An error occurred while writing '" + new_file + "'");
	}
	file.close();
	_session->addImage(path, _matchingBlock->getRoot());
	_msg = "   ✅📄 File " + new_file + " was successfully uploaded";
}

void	Response::_handleCgi()
{
	std::vector<std::string>::const_iterator ite;

	_fillCgiMetavariables();

	CgiHandler	cgi(*this);
	try {
		if (!cgi.getCgiOutput(_body)) {
			_is_cgi = false;
			throw (INTERNAL_SERVER_ERROR);
		}
	} catch (const std::runtime_error& e) {
		std::cerr << RED << "Exception in handlecgi : " << e.what() << RESET << NL;
		throw (INTERNAL_SERVER_ERROR);
	}
	/* If the request was successfull, we add uploaded files in the gallery */
	if (_statusCode >= 200 && _statusCode < 300)
	{
		for (ite = _cgiFilenames.begin(); ite != _cgiFilenames.end(); ite++)
		{
			std::cout << YELLOW << *ite << RESET << NL;
			_session->addImage(*ite, _matchingBlock->getRoot());
		}
	}
}

/* Perform resource-specific processing on the request payload. */
void	Response::_runPostMethod()
{
	std::ofstream		ofs;

	// _statusCode = BAD_REQUEST;
	_is_cgi = _isCgi(_builtPath);
	if (_is_cgi)
	{
		/* process cgi */
		_handleMultipartContentCgi(_request->getBody());
		return (_handleCgi());
	}
	// if (_matchingBlock->getUploadPath().empty())
	if (_uploadPath.empty())
	{
		// setStatusCode ??
		return ;
	}
	if (_contentTypeIsUrlencoded())
	{
		// setStatusCode ?
		return ;
	}
	if (_contentTypeIsMultipart())
	{
		return (_handleMultipartContent(_request->getBody()));
	}
	_writeFileContent(_builtPath, _request->getBody());
}

/******************************************************************************/
/*                               DELETE METHOD                                */
/******************************************************************************/

bool	Response::_deletePurchase(const std::string& uri)
{
	std::string	id;

	if (uri.find("/form_delete/") == 0)
	{
		id = uri.substr(13);
		if (_session->deletePurchase(id, &_msg))
		{
			return (true);
		}
	}
	return (false);
}

/* Remove all current representations of the target resource. */
void	Response::_runDeleteMethod()
{
	int	ret;

	if (_deletePurchase(_request->getPath()))
		return ;
	ret = std::remove(_builtPath.c_str());
	if (ret)
	{
		/* Error case */
		throw (_findErrorCode());
	}
 	/* Successfull case */
	setStatusCode(NO_CONTENT);
	_msg = "   ❎📄 Resource " + _builtPath + " was successfully uploaded";
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
	pos = _builtPath.rfind(".");
	if (pos != std::string::npos)
	{
		typeExtension = _builtPath.substr(pos);
	}
	return (g_mimeType[typeExtension]);
}

/* The keep-alive directive indicates that the client wants the HTTP Connection
to persist and remain open after the current transaction is complete.
This is the default setting for HTTP/1.1 requests. */
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

/******************************************************************************/
/*                                  PATH                                      */
/******************************************************************************/

void	Response::_generateAutoindex(const std::string& path)
{
	Autoindex	autoindex(path);

	_body = autoindex.getIndexPage();
	_headers["Content-Type"] = g_mimeType[".html"];
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
			// *path += "/" + *currentIndex;
			*path += *currentIndex;
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

void	Response::_handleSlash(std::string* path, const std::string& uri)
{
	if (*(path->rbegin()) == '/' && *(uri.begin()) == '/')
		path->erase(path->length() - 1);
	else if (*(path->rbegin()) != '/' && *(uri.begin()) != '/')
		*path += "/";
	if (*(path->begin()) == '/')
		path->insert(path->begin(), '.');
}

/* If a request ends with a slash, NGINX treats it as a request
for a directory and tries to find an index file in the directory. */
std::string		Response::_buildPath()
{
	std::string		path;
	std::string		uri;

	uri = _request->getPath();
	if (_locationPath != "")
		uri.erase(0, _locationPath.length());
	if (_hasUploadPathDirective())
		_uploadPath = _matchingBlock->getUploadPath();
	path = _matchingBlock->getRoot();
	_handleSlash(&path, uri);
	path += uri;
	if (path[0] == '/')
		path.insert(path.begin(), '.');
	if (pathIsDirectory(path))
		_handleDirectoryPath(&path);
	_builtPath = path;
	return (path);
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

bool	Response::_hasUploadPathDirective()
{
	return ((_method == POST || _method == DELETE)
		&& !_matchingBlock->getUploadPath().empty());
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

bool	Response::_contentTypeIsMultipart()
{
	size_t pos;

	pos = _request->getHeader("content-type").find("multipart/form-data");
	return (pos != std::string::npos);
}

bool	Response::_contentTypeIsUrlencoded()
{
	size_t pos;

	pos = _request->getHeader("content-type").find("application/x-www-form-urlencoded");
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

int		Response::getFd() const {
	return (_fd);
}

std::string		Response::getCgiProgram() const {
	return (_cgipath);
}

std::string		Response::getCgiName() const {
	return (_cgiscript);
}

std::string		Response::getCgiExtra() const {
	return (_cgiextra);
}

std::string		Response::getCgiQuery() const {
	return (_cgiquery);
}

Env*		Response::getEnv() const {
	return (_env);
}

std::string		Response::getBuiltPath() const
{
	return (_builtPath);
}

std::string		Response::getMsgToDisplay() const
{
	return (_msg);
}

/******************************************************************************/
/*                                    CGI                                     */
/******************************************************************************/

/* Check if we need to call cgi */
bool	Response::_isCgi(const std::string& path)
{
	std::string		extension;
	size_t			pos;

	if (_matchingBlock->getCgi().empty())
		return (false);
	pos = _parsePosCgiExtension(path);
	if (pos != std::string::npos)
	{
		_parseCgiUrl(pos);
		return (true);
	}
	return (false);
}

size_t	Response::_parsePosCgiExtension(std::string path) {

	std::string		extension;
	size_t			pos;

	for (pos = path.find("."); pos != std::string::npos; pos = path.find(".")) {
		extension = path.substr(pos + 1, path.find("/"));
		if (_matchingBlock->findCgi(extension) != "")
		{
			_cgipath = _matchingBlock->findCgi(extension);
			// std::cout	<< YELLOW << "in cgi path = " << path
			// 			<< " | extension : " << extension
			// 			<< " | path = " << _cgipath << RESET << NL;
			break ;
		}
		path.erase(0, extension.length() + 1);
	}
	return (pos);
}

void   Response::_parseCgiUrl(size_t pos_extension) {
	std::string		path = _request->getPath();

	size_t		pos_end_extension = path.find('/', pos_extension);
	size_t		pos_cgi = path.find_last_of('/', pos_extension);
	if (pos_cgi == std::string::npos)
		return ;
	std::string	cgi = path.substr(pos_cgi + 1, pos_end_extension);
	_cgiscript = cgi;
	_cgiquery = _request->getQuery();
	_cgiextra = ""; // TODO : get extra
}

/* Retrieve info from request and put them in env */
void	Response::_fillCgiMetavariables() {
	_env->addParam("SERVER_PROTOCOL", _request->getHttpProtocol());
	_env->addParam("CONTENT_LENGTH", convertNbToString(_request->getBodySize()));
	_env->addParam("CONTENT_TYPE", _request->getHeader("content-type"));
	_env->addParam("REQUEST_METHOD", _request->getMethodStr());
	_env->addParam("QUERY_STRING", getCgiQuery());
	_env->addParam("SCRIPT_NAME", _request->getPath());
	_env->addParam("SCRIPT_FILENAME", _translateCgiName());
	_env->addParam("PATH_INFO", getCgiExtra()); // TODO: parse esc char
	_env->addParam("PATH_TRANSLATED", ""); //TODO PATH_INFO translated :
	_env->addParam("REMOTE_ADDR", "localhost"); /* TODO: inet_hton ? Recode epoll ? */
	_env->addParam("SERVER_PORT", convertNbToString(_request->getPort()));
	_env->addParam("REMOTE_IDENT", ""); // optional

	//std::cerr << RED << "Query: " << getCgiQuery() << RESET << NL;
	_env->addParam("HTTP_ACCEPT", _request->getHeader("accept"));
	_env->addParam("HTTP_ACCEPT_LANGUAGE", _request->getHeader("accept-language"));
	_env->addParam("HTTP_USER_AGENT", _request->getHeader("user-agent"));
	_env->addParam("HTTP_COOKIE", _request->getHeader("cookie"));
	_env->addParam("HTTP_REFERER", _request->getHeader("referer"));
	_env->addParam("REDIRECT_STATUS", getStatusCodeStr());

}

std::string		Response::_translateCgiName() const {
	std::string     translation(std::string(WEBSERV_PATH));
	std::string		relativePath(getBuiltPath());

	if (relativePath[0] == '.')
		relativePath.erase(0, 1);
	else if (relativePath[0] != '/')
		relativePath.insert(0, "/");
	translation += relativePath;
	return (translation);
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

void	Response::_fillCookieHeader()
{
	_response += _session->getCookieHeader();
}

Session*	Response::getSession() const
{
	return (_session);
}
