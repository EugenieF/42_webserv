#include "Cookies.hpp"

Cookies::Cookies()
{
	_time = std::time(NULL);
}

Cookies::Cookies(const std::string& sessionId)
{
	_time = std::time(NULL);
	setCookie("SID", sessionId);
}

Cookies::Cookies(const Cookies& other)
{
	*this = other;
}

Cookies&		Cookies::operator=(const Cookies& other)
{
	if (this != &other)
	{
		_sessionId = other.getSessionId();
		_cookies = other.getCookies();
	}
	return (*this);
}

Cookies::~Cookies() {}

size_t	Cookies::size() const
{
	return (_cookies.size());
}

void	Cookies::setCookie(const std::string& name, const std::string& value)
{
	checkSizeCookie(name, value);
	_cookies[name] = value;
	if (name == "SID")
		_sessionId = value;
}

void	Cookies::setCookies(std::string header)
{
	std::string		name;
	std::string		value;
	size_t			pos;

	/* Limit Firefox = 150 cookies */
	for (size_t nbOfCookiess = 0; nbOfCookiess < 150; nbOfCookiess++)
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

void	Cookies::fillCookies(const Cookies& other)
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

void	Cookies::checkSizeCookie(const std::string& name, const std::string& value)
{
	size_t	totalSize;

	totalSize = name.size() + value.size() + 1;
	if (totalSize > 4096) /* Limit 4096 bytes */
		throw(BAD_REQUEST);
}

void	Cookies::display() const
{
	mapOfCookies::const_iterator	ite;

	std::cout << std::endl << LIGHT_YELLOW << "🍪 List of cookies:" << RESET << std::endl;
	for (ite = _cookies.begin(); ite != _cookies.end(); ite++)
		std::cout << "   > " << ite->first << "=" << ite->second << std::endl;
	std::cout << std::endl;
}

std::string		Cookies::getSessionId() const
{
	return (_sessionId);
}

Cookies::mapOfCookies&	Cookies::getCookies()
{
	return (_cookies);
}

const Cookies::mapOfCookies&	Cookies::getCookies() const
{
	return (_cookies);
}

bool	Cookies::isSet(const std::string& name)
{
	mapOfCookies::const_iterator	ite;

	ite = _cookies.find(name);
	return (ite != _cookies.end());
}

std::string	 Cookies::getCookie(const std::string& name)
{
	if (isSet(name))
		return (_cookies[name]);
	return ("");
}

bool	Cookies::isEmpty()
{
	return (_cookies.empty());
}

std::string	Cookies::setCookieHeader()
{
	std::string						header;
	mapOfCookies::const_iterator	ite;

	for (ite = _cookies.begin(); ite != _cookies.end(); ite++)
		header += "Set-Cookies: " + ite->first + "=" + ite->second + "\r\n";
	return (header);
}

bool	Cookies::sessionIsAlive()
{
	return (std::time(NULL) - _time < SESSION_TIMEOUT);
}

void	Cookies::updateTime()
{
	_time = std::time(NULL);
}

std::string&	Cookies::operator[](const std::string& name)
{
	return (_cookies[name]);
}
