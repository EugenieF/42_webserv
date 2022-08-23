#include "Block.hpp"

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
	initAllowedMethods();
	for (int i = 0; i < ALLOWED_METHODS_COUNT; i++)
		_methods[i] = false;
	if (!getuid())
		setPort(80);
}

Block::~Block()
{
	if (isServerBlock())
		deleteLocations();
}

bool	Block::isServerBlock()
{
	return (_context == SERVER);
}

bool	Block::isLocationBlock()
{
	return (_context == LOCATION);
}

void	Block::initAllowedMethods()
{
	_allowedMethods[GET] = "GET";
	_allowedMethods[POST] = "POST";
	_allowedMethods[DELETE] = "DELETE";
}

void	Block::setRoot(const std::string &root)
{
	_root = root;
}

const std::string&	Block::getRoot() const
{
	return (_root);
}

void	Block::setIndex(const std::string &index)
{
	_indexes.insert(_indexes.end(), index);
}

Block::listOfStrings&	Block::getIndexes()
{
	return (_indexes);
}

void	Block::setAutoindex(bool value)
{
	_autoindex = value;
}

bool	Block::getAutoindex() const
{
	return (_autoindex);
}

void	Block::setClientBodyLimit(size_t maxSize)
{
	_clientBodyLimit = maxSize;
}

size_t	Block::getClientBodyLimit() const
{
	return (_clientBodyLimit);
}

void	Block::setCgi(const std::string &extension, const std::string &path)
{
	_cgiExt = extension;
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

void	Block::setErrorPage(const std::string &codeStr, const std::string &page)
{
	int	code;

	code = atoi(codeStr.c_str());
	_errorPages[code] = page;
}

// int		Block::getErrorCode()
// {
// 	return (_errorPages);
// }

// const std::string&	Block::getErrorPage()
// {

// }

void	Block::setRedirection(int code, const std::string &uri)
{
	_redirectCode = code;
	_redirectUri = uri;
}

int		Block::getRedirectCode()
{
	return (_redirectCode);
}

const std::string&	Block::getRedirectUri()
{
	return (_redirectUri);
}

void	Block::setUploadPath(const std::string &path)
{
	_uploadPath = path;
}

const std::string	&Block::getUploadPath() const
{
	return (_uploadPath);
}

void	Block::setMethod(t_method method)
{
	_methods[method] = true;
}

bool	Block::isAllowedMethod(std::string str)
{
	for (int i = 0; i < ALLOWED_METHODS_COUNT; i++)
	{
		if (str == _allowedMethods[i])
			return (true);
	}
	return (false);
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

bool	Block::insertLocation(std::string path, blockPtr newLocation)
{
	std::pair<listOfLocations::const_iterator, bool>	ret;

	ret = _locations.insert(std::make_pair(path, newLocation));
	_currentLocation = ret.first;
	return (ret.second);
}

void	Block::setName(const std::string &name)
{
	_serverNames.insert(_serverNames.end(), name);
}

void	Block::setPort(int port)
{
	_port = port;
}

void	Block::setPort(std::string portStr)
{
	int	port;

	port = atoi(portStr.c_str());
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

void	Block::deleteLocations()
{
	for (_currentLocation = _locations.begin(); _currentLocation != _locations.end(); _currentLocation++)
	{
		std::cout << ORANGE << " xxx Delete a location xxx" << RESET << std::endl;
		delete (_currentLocation->second);
	}
}

Block::blockPtr		Block::getCurrentLocation()
{
	return (_currentLocation->second);
}

void	Block::displayParams(int num)
{
	listOfLocations::const_iterator	ite;

	std::cout << std::endl << " SERVER " << num << ": " << std::endl;
	std::cout << "  ‣ Names: ";
	displayListOfStrings(_serverNames);
	std::cout << "  ‣ Port: " << getHost() << ":" << getPort() << std::endl;
	displayBlockDirectives(SERVER);
	for (ite = _locations.begin(); ite != _locations.end(); ite++)
	{
		std::cout << std::endl << "     Location " << ite->first << ": " << std::endl;
		ite->second->displayBlockDirectives(LOCATION);
	}
}
