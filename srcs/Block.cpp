#include "Block.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Block::Block():
	_context(NONE),
	_serverNames(0),
	_port(8000),
	_host("0.0.0.0"),
	_root(DEFAULT_ROOT),
	_indexes(0),
	_autoindex(false),
	_clientBodyLimit(DEFAULT_CLIENT_BODY_LIMIT),
	_cgiExt(""),
	_cgiPath(""),
	_redirectCode(0),
	_redirectUri(""),
	_uploadPath("")
{
	_initAllowedMethods();
	for (int i = 0; i < ALLOWED_METHODS_COUNT; i++)
		_methods[i] = false;
	if (!getuid())
		setPort(80);
}

Block::Block(const Block& other):
	_context(other.getContext()),
	_serverNames(other.getServerNames()),
	_port(other.getPort()),
	_host(other.getHost()),
	_root(other.getRoot()),
	_indexes(other.getIndexes()),
	_autoindex(other.getAutoindex()),
	_clientBodyLimit(other.getClientBodyLimit()),
	_cgiExt(other.getCgiExt()),
	_cgiPath(other.getCgiPath()),
	_redirectCode(other.getRedirectCode()),
	_redirectUri(other.getRedirectUri()),
	_uploadPath(other.getUploadPath())
{
	*this = other;
}

Block::~Block()
{
	if (isServerBlock())
		_deleteLocations();
}

Block&	Block::operator=(const Block& other)
{
	if (this != &other)
	{
		_context = other.getContext();
		_serverNames = other.getServerNames();
		_port = other.getPort();
		_host = other.getHost();
		_root = other.getRoot();
		_indexes = other.getIndexes();
		_autoindex = other.getAutoindex();
		_clientBodyLimit = other.getClientBodyLimit();
		_cgiExt = other.getCgiExt();
		_cgiPath = other.getCgiPath();
		_redirectCode = other.getRedirectCode();
		_redirectUri = other.getRedirectUri();
		_uploadPath = other.getUploadPath();
	}
	return (*this);
}

Block*		Block::getMatchingBlock(std::string const& host, std::string const& path)
{
	std::cout << BLUE << "Host : " << host << " | path : " << path << RESET << std::endl;
	return (this);
}

/******************************************************************************/
/*                              SERVER_NAME                                   */
/******************************************************************************/

void	Block::setName(const std::string& name)
{
	_serverNames.insert(_serverNames.end(), name);
}

Block::listOfStrings	Block::getServerNames() const
{
	return (_serverNames);
}

/******************************************************************************/
/*                                 LISTEN                                     */
/******************************************************************************/

void	Block::setPort(int port)
{
	_port = port;
}

int		Block::getPort() const
{
	return (_port);
}

void	Block::setHost(const std::string &host)
{
	_host = host;
}

const std::string&	Block::getHost() const
{
	return (_host);
}

/******************************************************************************/
/*                                  ROOT                                      */
/******************************************************************************/

void	Block::setRoot(const std::string &root)
{
	_root = root;
}

const std::string&	Block::getRoot() const
{
	return (_root);
}

/******************************************************************************/
/*                                  INDEX                                     */
/******************************************************************************/

void	Block::setIndex(const std::string &index)
{
	_indexes.insert(_indexes.end(), index);
}

Block::listOfStrings	Block::getIndexes() const
{
	return (_indexes);
}

/******************************************************************************/
/*                                AUTOINDEX                                   */
/******************************************************************************/

void	Block::setAutoindex(bool value)
{
	_autoindex = value;
}

bool	Block::getAutoindex() const
{
	return (_autoindex);
}

/******************************************************************************/
/*                            CLIENT_MAX_BODY_SIZE                            */
/******************************************************************************/

void	Block::setClientBodyLimit(size_t size)
{
	_clientBodyLimit = size;
}

size_t	Block::getClientBodyLimit() const
{
	return (_clientBodyLimit);
}

/******************************************************************************/
/*                                   CGI                                      */
/******************************************************************************/

void	Block::setCgiExt(const std::string& extension)
{
	_cgiExt = extension;
}

void	Block::setCgiPath(const std::string& path)
{
	_cgiPath = path;
}

const std::string&	Block::getCgiExt() const
{
	return (_cgiExt);
}

const std::string&	Block::getCgiPath() const
{
	return (_cgiPath);
}

/******************************************************************************/
/*                                ERROR_PAGE                                  */
/******************************************************************************/

void	Block::setErrorPage(int code, const std::string& page)
{
	_errorPages[code] = page;
}

Block::listOfErrorPages		Block::getErrorPages()
{
	return (_errorPages);
}

std::string		Block::getErrorPage(int statusCode)
{
	Block::listOfErrorPages::const_iterator	ite;
	std::string								errorPage;

	errorPage = "";
	ite = _errorPages.find(statusCode);
	if (ite != _errorPages.end())
		errorPage = ite->second;
	return (errorPage);
}

/******************************************************************************/
/*                                 REDIRECT                                   */
/******************************************************************************/

void	Block::setRedirection(int code, const std::string& uri)
{
	_redirectCode = code;
	_redirectUri = uri;
}

int		Block::getRedirectCode()
{
	return (_redirectCode);
}

const std::string&	Block::getRedirectUri() const
{
	return (_redirectUri);
}

int		Block::getRedirectCode() const
{
	return (_redirectCode);
}

/******************************************************************************/
/*                              UPLOAD_PATH                                   */
/******************************************************************************/

void	Block::setUploadPath(const std::string& path)
{
	_uploadPath = path;
}

const std::string	&Block::getUploadPath() const
{
	return (_uploadPath);
}

/******************************************************************************/
/*                             ALLOWED_METHOD                                 */
/******************************************************************************/

void	Block::setMethod(Block::t_method method)
{
	_methods[method] = true;
}

void	Block::setMethod(const std::string& str)
{
	for (int i = 0; i < ALLOWED_METHODS_COUNT; i++)
	{
		if (str == _allowedMethods[i])
			_methods[i] = true;
	}
}

bool	Block::isMethod(const std::string& str)
{
	return (str == "GET" || str == "POST" || str == "DELETE");
}

bool	Block::isAllowedMethod(Block::t_method method)
{
	return (_methods[method]);
}

bool	Block::isAllowedMethod(const std::string& str)
{
	for (int i = 0; i < ALLOWED_METHODS_COUNT; i++)
	{
		if (str == _allowedMethods[i])
			return (true);
	}
	return (false);
}

Block::t_method	Block::getMethod(const std::string& str) const
{
	for (int i = 0; i < ALLOWED_METHODS_COUNT; i++)
	{
		if (str == _allowedMethods[i])
			return (i);
	}
	return (NO_METHOD);
}

std::string const&	Block::getMethod(Block::t_method method) const
{
	return (_allowedMethods[method]);
}

/******************************************************************************/
/*                                LOCATION                                    */
/******************************************************************************/

bool	Block::insertLocation(const std::string& path, blockPtr newLocation)
{
	std::pair<listOfLocations::const_iterator, bool>	ret;

	ret = _locations.insert(std::make_pair(path, newLocation));
	_currentLocation = ret.first;
	return (ret.second);
}

Block::blockPtr		Block::getCurrentLocation()
{
	return (_currentLocation->second);
}

Block::listOfLocations	Block::getLocations() const
{
	return (_locations);
}

int		Block::getNbOfLocations() const
{
	int									count;
	listOfLocations::const_iterator		ite;

	count = 0;
	for (ite = _locations.begin(); ite != _locations.end(); ite++)
		count++;
	return (count);
}

/******************************************************************************/
/*                                 CONTEXT                                    */
/******************************************************************************/

t_context		Block::getContext() const
{
	return (_context);
}

void	Block::setContext(t_context context)
{
	_context = context;
}

/******************************************************************************/
/*                                  UTILS                                     */
/******************************************************************************/

bool	Block::isServerBlock()
{
	return (_context == SERVER);
}

bool	Block::isLocationBlock()
{
	return (_context == LOCATION);
}

void	Block::_initAllowedMethods()
{
	_allowedMethods[GET] = "GET";
	_allowedMethods[POST] = "POST";
	_allowedMethods[DELETE] = "DELETE";
}

void	Block::_deleteLocations()
{
	for (_currentLocation = _locations.begin(); _currentLocation != _locations.end(); _currentLocation++)
	{
		// std::cout << ORANGE << " xxx Delete a location xxx" << RESET << std::endl;
		delete (_currentLocation->second);
	}
}

/******************************************************************************/
/*                                 DISPLAY                                    */
/******************************************************************************/

void	Block::displayParams(int num)
{
	listOfLocations::const_iterator	ite;

	std::cout << std::endl << " SERVER " << num << ": " << std::endl;
	std::cout << "  ‣ Names: ";
	displayListOfStrings(_serverNames);
	std::cout << "  ‣ Listen: " << getHost() << ":" << getPort() << std::endl;
	displayBlockDirectives(SERVER);
	for (ite = _locations.begin(); ite != _locations.end(); ite++)
	{
		std::cout << std::endl << "     Location " << ite->first << ": " << std::endl;
		ite->second->displayBlockDirectives(LOCATION);
	}
}

void	Block::displayBlockDirectives(t_context context)
{
	std::string indent;

	indent = (context == SERVER) ? "  " : "     "; 
	std::cout << indent << "‣ Root: " << getRoot() << std::endl;
	std::cout << indent << "‣ Index: ";
	displayListOfStrings(_indexes);
	std::cout << indent << "‣ Autoindex: " << getAutoindex() << std::endl;
	std::cout << indent << "‣ Body limit: " << getClientBodyLimit() << std::endl;
	std::cout << indent << "‣ Upload path: " << getUploadPath() << std::endl;
	std::cout << indent << "‣ Redirection: " << getRedirectCode() << " " << getRedirectUri() << std::endl;
	std::cout << indent << "‣ Cgi: " << getCgiExt() << " " << getCgiPath() << std::endl;
	// std::cout << "  ‣ Error page: " << getErrorCode() << " " << getErrorPage() << std::endl;
}

void	Block::displayListOfStrings(listOfStrings list)
{
	listOfStrings::const_iterator	ite;

	for (ite = list.begin(); ite != list.end(); ite++)
		std::cout << *ite << " ";
	std::cout << std::endl;
}
