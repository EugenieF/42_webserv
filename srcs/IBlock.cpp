#include "IBlock.hpp"

IBlock::IBlock():
	_root(DEFAULT_ROOT),
	_autoindex(false),
	_clientBodyLimit(DEFAULT_CLIENT_BODY_LIMIT)
{}

IBlock::~IBlock() {}

void	IBlock::setRoot(const std::string &root)
{
	_root = root;
}

const std::string&	IBlock::getRoot() const
{
	return (_root);
}

void	IBlock::setIndex(const std::string &index)
{
	_indexes.insert(_indexes.end(), index);
}

void	IBlock::setAutoindex(bool value)
{
	_autoindex = value;
}

bool	IBlock::getAutoindex() const
{
	return (_autoindex);
}

void	IBlock::setClientBodyLimit(size_t maxSize)
{
	_clientBodyLimit = maxSize;
}

size_t	IBlock::getClientBodyLimit() const
{
	return (_clientBodyLimit);
}

void	IBlock::setCgi(const std::string &extension, const std::string &path)
{
	_cgiExtension = extension;
	_cgiPath = path;
}

void	IBlock::setErrorPage(int code, const std::string &page)
{
	_errorPages[code] = page;
}

void	IBlock::setRedirection(const std::string &uri, int code)
{
	_redirectCode = code;
	_redirectUri = uri;
}
