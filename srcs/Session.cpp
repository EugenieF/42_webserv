#include "Session.hpp"

/******************************************************************************/
/*                              CLASS SESSION                                 */
/******************************************************************************/

Session::Session(const std::string& id)
{
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
		_id = other.getId();
		_cookies = other.getCookies();
		_order = other.getOrder();
	}
	return (*this);
}

Session::~Session() {}

std::string			Session::getId() const
{
	return (_id);
}

Cookies*	Session::getCookies()
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

/******************************************************************************/
/*                           CLASS SESSION HANDLER                            */
/******************************************************************************/

SessionHandler::SessionHandler()
{
	srand(std::time(NULL));
}

Cookie*	SessionHandler::_newSession()
{
	std::string	newId;
	Cookie*		newCookie;

	/* init creation time */
	newId = _generateSessionId();
	newCookie = new Cookie(newId);
	_sessions[newId] = newCookie;
	return (newCookie);
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
	while (findSession(sessionId) != _sessions.end())
		sessionId = _generateRandomString(SESSION_ID_LENGTH);
	DEBUG("SESSION ID = " + sessionId);
	return (sessionId);
}

std::vector<Session>&	SessionHandler::getSessions()
{
	return (_sessions);
}

const std::vector<Session>&	SessionHandler::getSessions() const
{
	return (_sessions);
}

std::vector<Session>::const_iterator	SessionHandler::findSession(std::string sessionId)
{
	std::vector<Session>::const_iterator	ite;

	for (ite = _sessions.begin(); ite != _sessions.end(); ite++)
	{
		if (ite->getId() == sessionId)
			return (ite);
	}
}

Cookies*	SessionHandler::getSessionCookies(std::string sessionId)
{
	std::vector<Session>::const_iterator	ite;

	ite = findSession(sessionId);	
	if (ite != _sessions.end())
		return (ite->getCookies());
}

SessionHandler::listOfPurchases		SessionHandler::getSessionOrder(std::string sessionId)
{
	std::vector<Session>::const_iterator	ite;

	for (ite = _sessions.begin(); ite != _sessions.end(); ite++)
	{
		if (ite->getId() == sessionId)
			return (ite->getOrder());
	}
}

Cookie*		SessionHandler::_findSession(std::string sessionId)
{
	mapOfSessions::iterator		ite;
	Cookie*						cookies;
	
	ite = _sessions.find(sessionId);
	if (ite != _sessions.end())
	{
		cookies = ite->second;
		if (cookies->sessionIsAlive())
		{
			cookies->updateTime();
			return (cookies);
		}
		_deleteSession(ite);
	}
	return (_newSession());
}

Cookies*	SessionHandler::lookupSession(const Cookie& requestCookies)
{
	std::string						sessionId;
	Cookie*							cookies;

	sessionId = requestCookies.getSessionId();
	if (sessionId == "")
		cookies = _newSession();
	else
		cookies = _findSession(sessionId);
	cookies->fillCookies(requestCookies);
	if (cookies->size() > 150)
		throw(PAYLOAD_TOO_LARGE);
	// cookies->display();
	return (cookies);
}
