#include "IRules.hpp"

IRules::IRules():
	_root(DEFAULT_ROOT),
	_autoindex(false),
	_clientBodyLimit(DEFAULT_CLIENT_BODY_LIMIT),
	_uploadPath("")
{
	initAllowedMethods();
	for (int i = 0; i < ALLOWED_METHODS_COUNT; i++)
		_methods[i] = false;
}

IRules::~IRules() {}

void	IRules::initAllowedMethods()
{
	_allowedMethods[GET] = "GET";
	_allowedMethods[POST] = "POST";
	_allowedMethods[DELETE] = "DELETE";
}

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

IRules::listOfStrings&	IRules::getIndexes()
{
	return (_indexes);
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
	_cgiExt = extension;
	_cgiPath = path;
}

const std::string&	IRules::getCgiExt() const
{
	return (_cgiExt);
}

const std::string&	IRules::getCgiPath() const
{
	return (_cgiPath);
}

void	IRules::setErrorPage(int code, const std::string &page)
{
	_errorPages[code] = page;
}

// int		IRules::getErrorCode()
// {
// 	return (_errorPages);
// }

// const std::string&	IRules::getErrorPage()
// {

// }

void	IRules::setRedirection(int code, const std::string &uri)
{
	_redirectCode = code;
	_redirectUri = uri;
}

int		IRules::getRedirectCode()
{
	return (_redirectCode);
}

const std::string&	IRules::getRedirectUri()
{
	return (_redirectUri);
}

void	IRules::setUploadPath(const std::string &path)
{
	_uploadPath = path;
}

const std::string	&IRules::getUploadPath() const
{
	return (_uploadPath);
}

void	IRules::setMethod(t_method method)
{
	_methods[method] = true;
}

bool	IRules::isAllowedMethod(std::string str)
{
	for (int i = 0; i < ALLOWED_METHODS_COUNT; i++)
	{
		if (str == _allowedMethods[i])
			return (true);
	}
	return (false);
}

void	IRules::displayRulesParams()
{
	std::cout << "  ‣ Root: " << getRoot() << std::endl;
	std::cout << "  ‣ Index: ";
	displayListOfStrings(_indexes);
	std::cout << "  ‣ Autoindex: " << getAutoindex() << std::endl;
	std::cout << "  ‣ Body limit: " << getClientBodyLimit() << std::endl;
	std::cout << "  ‣ Upload path: " << getUploadPath() << std::endl;
	std::cout << "  ‣ Redirection: " << getRedirectCode() << " " << getRedirectUri() << std::endl;
	std::cout << "  ‣ Cgi: " << getCgiExt() << " " << getCgiPath() << std::endl;
	// std::cout << "  ‣ Error page: " << getErrorCode() << " " << getErrorPage() << std::endl;
}

void	IRules::displayListOfStrings(listOfStrings list)
{
	listOfStrings::const_iterator	ite;

	for (ite = list.begin(); ite != list.end(); ite++)
		std::cout << *ite << " ";
	std::cout << std::endl;

}
