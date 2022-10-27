#include "Session.hpp"

/******************************************************************************/
/*                              CLASS SESSION                                 */
/******************************************************************************/

Session::Session()
{
	updateTime();
}

Session::Session(const std::string& id)
{
	updateTime();
	_id = id;
}

Session::Session(const Session& other)
{
	*this = other;
}

Session&	Session::operator=(const Session& other)
{
	if (this != &other)
	{
		_time = other.getTime();
		_id = other.getId();
		_cookies = other.getCookies();
		_order = other.getOrder();
	}
	return (*this);
}

Session::~Session() {}

size_t	Session::getTime() const
{
	return (_time);
}

std::string			Session::getId() const
{
	return (_id);
}

Session::listOfCookies&	Session::getCookies()
{
	return (_cookies);
}

const Session::listOfCookies&	Session::getCookies() const
{
	return (_cookies);
}

Session::listOfPurchases&		Session::getOrder()
{
	return (_order);
}

const Session::listOfPurchases&		Session::getOrder() const
{
	return (_order);
}

bool	Session::sessionIsAlive()
{
	return (std::time(NULL) - _time < SESSION_TIMEOUT);
}

void	Session::updateTime()
{
	_time = std::time(NULL);
}

void	Session::fillCookies(const listOfCookies& cookies)
{
	listOfCookies::const_iterator	ite;
	size_t							count;

	for (ite = cookies.begin(), count = 0; ite != cookies.end(); ite++, count++)
	{
		addCookie(ite->getName(), ite->getValue());
		if (count > 150)
			throw(PAYLOAD_TOO_LARGE);
	}
}

void	Session::addCookie(const std::string& name, const std::string& value)
{
	_cookies.insert(_cookies.end(), Cookie(name, value));
}

void    Session::addPurchase(
	const std::string& name, const std::string& hamster, const std::string& color)
{
    _order.insert(_order.end(), Purchase(name, hamster, color));
}

void	Session::deletePurchase(listOfPurchases::iterator ite)
{
	_order.erase(ite);
}

std::string		Session::getCookieHeader()
{
	std::string						header;
	listOfCookies::const_iterator	ite;

	for (ite = _cookies.begin(); ite != _cookies.end(); ite++)
		header += "Set-Cookies: " + ite->getName() + "=" + ite->getValue() + "\r\n";
	return (header);
}

/******************************************************************************/
/*                           CLASS SESSION HANDLER                            */
/******************************************************************************/

SessionHandler::SessionHandler()
{
	srand(std::time(NULL));
}

Session&	SessionHandler::_newSession()
{
	std::string		newId;

	newId = _generateSessionId();
	Session	newSession(newId);
	_sessions.insert(_sessions.end(), newSession);
	return (newSession);
}	

std::string 	SessionHandler::_generateRandomString(size_t length)
{
	std::string		charset;
	int				pos;
	size_t			maxIndex;
	std::string		randomStr;

	charset = "0123456789abcdefghijklmnopqrstuvwxyz";
	maxIndex = charset.size() - 1;
	while(randomStr.size() != length)
	{
		pos = ((rand() % maxIndex));
		randomStr += charset.substr(pos, 1);
	}
	return (randomStr);
}

/* Generate unique Identifier for the new Session */
std::string		SessionHandler::_generateSessionId()
{
	std::string		sessionId;

	sessionId = _generateRandomString(SESSION_ID_LENGTH);
	// while (findSession(sessionId) != _sessions.end())
	// 	sessionId = _generateRandomString(SESSION_ID_LENGTH);
	DEBUG("SESSION ID = " + sessionId);
	return (sessionId);
}

SessionHandler::listOfSessions&	SessionHandler::getSessions()
{
	return (_sessions);
}

const SessionHandler::listOfSessions&	SessionHandler::getSessions() const
{
	return (_sessions);
}

Session&	SessionHandler::findSession(const std::string& sessionId)
{
	listOfSessions::iterator	ite;

	for (ite = _sessions.begin(); ite != _sessions.end(); ite++)
	{
		if (ite->getId() == sessionId)
		{
			if (ite->sessionIsAlive())
			{
				ite->updateTime();
				return (*ite);
			}
			_sessions.erase(ite);
			break ;
		}
	}
	return (_newSession());
}

std::string	SessionHandler::getCookieSID(const listOfCookies& cookies)
{
	listOfCookies::const_iterator	ite;

	for (ite = cookies.begin(); ite != cookies.end(); ite++)
	{
		if (ite->getName() == "SID")
			return (ite->getValue());
	}
	return ("");
}

Session&	SessionHandler::lookupSession(const listOfCookies& requestCookies)
{
	std::string						sessionId;
	Session							session;

	sessionId = getCookieSID(requestCookies);
	if (sessionId == "")
		session = _newSession();
	else
		session = findSession(sessionId);
	session.fillCookies(requestCookies);
	// session.getCookies().display();
	return (session);
}
