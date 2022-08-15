#include "Location.hpp"

Location::Location(): _uploadPath("")
{
	for (int i = 0; i < ALLOWED_METHODS_COUNT; i++)
		_allowedMethods[i] = false;
}

Location::~Location() {}

void	Location::setUploadPath(const std::string &path)
{
	_uploadPath = path;
}

const std::string	&Location::getUploadPath() const
{
	return (_uploadPath);
}

void	Location::setAllowedMethod(t_method method)
{
	_allowedMethods[method] = true;
}

bool	Location::isAllowedMethod(t_method method)
{
	return (_allowedMethods[method]);
}
