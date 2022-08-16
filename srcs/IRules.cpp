#include "IRules.hpp"

IRules::IRules():
	_root(DEFAULT_ROOT),
	_autoindex(false),
	_clientBodyLimit(DEFAULT_CLIENT_BODY_LIMIT)
{}

IRules::~IRules() {}

void	IRules::setRoot(const std::string &root)
{
	_root = root;
}

const std::string&	IRules::getRoot() const
{
	return (_root);
}

void	IRules::setIndex(const std::string &index)
{
	_indexes.insert(_indexes.end(), index);
}

void	IRules::setAutoindex(bool value)
{
	_autoindex = value;
}

bool	IRules::getAutoindex() const
{
	return (_autoindex);
}

void	IRules::setClientBodyLimit(size_t maxSize)
{
	_clientBodyLimit = maxSize;
}

size_t	IRules::getClientBodyLimit() const
{
	return (_clientBodyLimit);
}

void	IRules::setCgi(const std::string &extension, const std::string &path)
{
	_cgiExtension = extension;
	_cgiPath = path;
}

void	IRules::setErrorPage(int code, const std::string &page)
{
	_errorPages[code] = page;
}

void	IRules::setRedirection(const std::string &uri, int code)
{
	_redirectCode = code;
	_redirectUri = uri;
}
