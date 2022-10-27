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
	_addCookie(SESSION_ID, id);
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
		if (ite->getName() != SESSION_ID)
			_addCookie(ite->getName(), ite->getValue());
		if (count > 150)
			throw(PAYLOAD_TOO_LARGE);
	}
}

void	Session::_addCookie(const std::string& name, const std::string& value)
{
	_cookies.insert(_cookies.end(), Cookie(name, value));
}

void    Session::_addPurchaseInOrder()
{
	_order.insert(_order.end(), Purchase(_purchase));
	_purchase.setName("");
	_purchase.setHamster("");
	_purchase.setColor("");
}

void	Session::completePurchase(const std::string& name, const std::string& content)
{
    if (name == "name") {
        _purchase.setName(content);
	}
    else if (name == "hamster") {
        _purchase.setHamster(content);
	}
    else if (name == "color") {
        _purchase.setColor(content);
	}
	if (_purchase.isComplete()) {
		_addPurchaseInOrder();
	}
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
		header += "Set-Cookie: " + ite->getName() + "=" + ite->getValue() + "\r\n";
	return (header);
}

void	Session::displayCookies() const
{
	listOfCookies::const_iterator	ite;

	std::cout << "List of Cookies :" << std::endl;
	for (ite = _cookies.begin(); ite != _cookies.end(); ite++)
		ite->display();
}

/******************************************************************************/
/*                           CLASS SESSION HANDLER                            */
/******************************************************************************/

SessionHandler::SessionHandler()
{
	srand(std::time(NULL));
}

SessionHandler::~SessionHandler()
{
	_deleteSessions();
}

void	SessionHandler::_deleteSessions()
{
	listOfSessions::iterator	ite;

	for (ite = _sessions.begin(); ite != _sessions.end(); ite++)
	{
		delete (*ite);
		*ite = NULL;
	}
	_sessions.clear();
}

void	SessionHandler::_deleteSession(listOfSessions::iterator ite)
{
	Session*	tmp;

	tmp = *ite;
	_sessions.erase(ite);
	delete tmp;
	tmp = NULL;
}

Session*	SessionHandler::_newSession()
{
	std::string		newId;
	Session*		newSession;

	newId = _generateSessionId();
	newSession = new Session(newId);
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
	while (_findSessionIte(sessionId) != _sessions.end())
		sessionId = _generateRandomString(SESSION_ID_LENGTH);
	DEBUG("SESSION ID = " + sessionId);
	return (sessionId);
}

SessionHandler::listOfSessions::iterator	SessionHandler::_findSessionIte(const std::string& sessionId)
{
	listOfSessions::iterator	ite;

	for (ite = _sessions.begin(); ite != _sessions.end(); ite++)
	{
		if ((*ite)->getId() == sessionId)
			break ;
	}
	return (ite);
}

Session*	SessionHandler::_findSession(const std::string& sessionId)
{
	listOfSessions::iterator	ite;

	for (ite = _sessions.begin(); ite != _sessions.end(); ite++)
	{
		if ((*ite)->getId() == sessionId)
		{
			if ((*ite)->sessionIsAlive())
			{
				(*ite)->updateTime();
				return (*ite);
			}
			_deleteSession(ite);
			break ;
		}
	}
	return (_newSession());
}

std::string	SessionHandler::_getCookieSID(const listOfCookies& cookies)
{
	listOfCookies::const_iterator	ite;

	for (ite = cookies.begin(); ite != cookies.end(); ite++)
	{
		if (ite->getName() == SESSION_ID)
			return (ite->getValue());
	}
	return ("");
}

Session*	SessionHandler::lookupSession(const listOfCookies& requestCookies)
{
	std::string			sessionId;
	Session				*session;

	sessionId = _getCookieSID(requestCookies);
	if (sessionId == "")
		session = _newSession();
	else
		session = _findSession(sessionId);
	session->fillCookies(requestCookies);
	session->displayCookies();
	return (session);
}

SessionHandler::listOfSessions&	SessionHandler::getSessions()
{
	return (_sessions);
}

const SessionHandler::listOfSessions&	SessionHandler::getSessions() const
{
	return (_sessions);
}
