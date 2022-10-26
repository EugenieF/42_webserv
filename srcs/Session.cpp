#include "Session.hpp"

/******************************************************************************/
/*                              CLASS SESSION                                 */
/******************************************************************************/

Session::Session(const std::string& id)
{
	_time = std::time(NULL);
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

listOfCookies&	Session::getCookies()
{
	return (_cookies);
}

const listOfCookies&	Session::getCookies() const
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

void	Session::setCookies(std::string header)
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
		addCookie(name, value);
		if (pos == std::string::npos)
			return ;
	}
	throw (PAYLOAD_TOO_LARGE);
}

void	Session::fillCookies(const listOfCookies& cookies)
{
	listOfCookies::iterator	ite;

	for (ite = cookies.begin(); ite != cookies.end(); ite++)
		_addCookie(ite->getName(), ite->getValue());
}

void	Session::addCookie(const std::string& name, const std::string& value)
{
	_cookies.insert(_cookies.end(), Cookie(name, value));
}

void    Session::addPurchase(const std::string& name, const std::string& hamster, const std::string& color)
{
    _order.insert(_order.end(), Purchase(name, hamster, color));
}

void	Session::deletePurchase(listOfPurchases::iterator ite)
{
	_order.erase(ite);
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
	while (findSession(sessionId) != _sessions.end())
		sessionId = _generateRandomString(SESSION_ID_LENGTH);
	DEBUG("SESSION ID = " + sessionId);
	return (sessionId);
}

Session::listOfSessions&	SessionHandler::getSessions()
{
	return (_sessions);
}

const Session::listOfSessions&	SessionHandler::getSessions() const
{
	return (_sessions);
}

//////////



Session&	SessionHandler::findSession(const std::string& sessionId)
{
	listOfSession::const_iterator	ite;

	for (ite = _sessions.begin(); ite != _sessions.end(); ite++)
	{
		if (ite->getId() == sessionId)
			return (*ite);
	}
}

listOfCookies	SessionHandler::getSessionCookies(std::string sessionId)
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
