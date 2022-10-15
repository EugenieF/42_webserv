#include "Cookie.hpp"

Cookie::Cookie() {}

Cookie::Cookie(const std::string& sessionId)
{
	setCookie("SID", sessionId);
}

Cookie::Cookie(const Cookie& other)
{
	*this = other;
}

Cookie&		Cookie::operator=(const Cookie& other)
{
	if (this != &other)
	{
		_sessionId = other.getSessionId();
		_cookies = other.getCookies();
	}
	return (*this);
}

Cookie::~Cookie() {}

void	Cookie::setCookie(const std::string& name, const std::string& value)
{
	_cookies[name] = value;
	if (name == "SID" && _sessionId == "")
	{
		std::cout << GREEN << "SESSION ID SET" << RESET << std::endl;
		_sessionId = value;
	}
}

void	Cookie::setCookies(std::string header)
{
	std::string		name;
	std::string		value;
	size_t			pos;

	while (1)
	{
		pos = header.find("=");
		if (pos == std::string::npos)
			throw(BAD_REQUEST);
		name = header.substr(0, pos);
		header.erase(0, pos + 1);
		pos = header.find("; ");
		value = header.substr(0, pos);
		header.erase(0, pos + 2);
		setCookie(name, value);
		if (pos == std::string::npos)
			break ;
	}
}

void	Cookie::fillCookies(const Cookie& other)
{
	mapOfCookies					cookies;
	mapOfCookies::const_iterator	ite;

	cookies = other.getCookies();
	for (ite = cookies.begin(); ite != cookies.end(); ite++)
		setCookie(ite->first, ite->second);
}

void	Cookie::display() const
{
	mapOfCookies::const_iterator	ite;

	for (ite = _cookies.begin(); ite != _cookies.end(); ite++)
		std::cout << ite->first << "=" << ite->second << std::endl;
}

std::string		Cookie::getSessionId() const
{
	return (_sessionId);
}

Cookie::mapOfCookies&	Cookie::getCookies()
{
	return (_cookies);
}

const Cookie::mapOfCookies&	Cookie::getCookies() const
{
	return (_cookies);
}

bool	Cookie::isSet(const std::string& name)
{
	mapOfCookies::const_iterator	ite;

	ite = _cookies.find(name);
	return (ite != _cookies.end());
}

std::string	 Cookie::getCookie(const std::string& name)
{
	if (isSet(name))
		return (_cookies[name]);
	return ("");
}

bool	Cookie::isEmpty()
{
	return (_cookies.empty());
}

std::string	Cookie::setCookieHeader()
{
	std::string						header;
	mapOfCookies::const_iterator	ite;

	for (ite = _cookies.begin(); ite != _cookies.end(); ite++)
		header += "Set-Cookie: " + ite->first + "=" + ite->second + "\r\n";
	return (header);
}
