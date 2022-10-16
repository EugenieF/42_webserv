#include "Cookie.hpp"

Cookie::Cookie()
{
	_time = getTime();
}

Cookie::Cookie(const std::string& sessionId)
{
	_time = getTime();
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

size_t	Cookie::size() const
{
	return (_cookies.size());
}

void	Cookie::setCookie(const std::string& name, const std::string& value)
{
	checkSizeCookie(name, value);
	_cookies[name] = value;
	if (name == "SID")
		_sessionId = value;
}

void	Cookie::setCookies(std::string header)
{
	std::string		name;
	std::string		value;
	size_t			pos;

	/* Limit Firefox = 150 cookies */
	for (size_t nbOfCookies = 0; nbOfCookies < 150; nbOfCookies++)
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
			return ;
	}
	throw (PAYLOAD_TOO_LARGE);
}

void	Cookie::fillCookies(const Cookie& other)
{
	mapOfCookies					cookies;
	mapOfCookies::const_iterator	ite;

	cookies = other.getCookies();
	for (ite = cookies.begin(); ite != cookies.end(); ite++)
	{
		if (ite->first != "SID")
			setCookie(ite->first, ite->second);
	}
}

void	Cookie::checkSizeCookie(const std::string& name, const std::string& value)
{
	size_t	totalSize;

	totalSize = name.size() + value.size() + 1;
	if (totalSize > 4096) /* Limit 4096 bytes */
		throw(BAD_REQUEST);
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

bool	Cookie::sessionIsAlive()
{
	return (getTime() - _time < SESSION_TIMEOUT);
}

void	Cookie::updateTime()
{
	_time = getTime();
}
