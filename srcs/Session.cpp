#include "Session.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Session::Session()
{
	srand(std::time(NULL));
}

Session::Session(const Session& other)
{
	*this = other;
}

Session&	Session::operator=(const Session& other)
{
	if (this != &other)
	{
		_sessions = other.getSessions();
	}
	return (*this);
}

Session::~Session()
{
	deleteSessions();
}

/******************************************************************************/
/*                                   LOOKUP                                   */
/******************************************************************************/

Cookie*		Session::_findSession(std::string sessionId)
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

Cookie*	Session::lookupSession(const Cookie& requestCookies)
{
	std::string						sessionId;
	Cookie*							cookies;

	sessionId = requestCookies.getSessionId();
	// std::cout << RED << "sessionId = '" << sessionId << "'" << RESET << std::endl;
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

/******************************************************************************/
/*                                   SETUP                                    */
/******************************************************************************/

Cookie*	Session::_newSession()
{
	std::string	newId;
	Cookie*		newCookie;

	/* init creation time */
	newId = _generateSessionId();
	newCookie = new Cookie(newId);
	_sessions[newId] = newCookie;
	return (newCookie);
}

std::string 	Session::_generateRandomString(size_t length)
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
std::string		Session::_generateSessionId()
{
	std::string		sessionId;

	sessionId = _generateRandomString(SESSION_ID_LENGTH);
	while (_sessions.find(sessionId) != _sessions.end())
		sessionId = _generateRandomString(SESSION_ID_LENGTH);
	return (sessionId);
}

/******************************************************************************/
/*                                  CLEANUP                                   */
/******************************************************************************/

void	Session::deleteSessions()
{
	mapOfSessions::iterator	currentSession;

	// for (session = _sessions.begin(); session != _sessions.end(); session++)
	// 	_deleteSession(session);
	currentSession = _sessions.begin();
	while (currentSession != _sessions.end())
	{
		_deleteSession(currentSession);
		currentSession = _sessions.begin();
	}
}

void	Session::_deleteSession(Session::mapOfSessions::iterator session)
{
	if (session->second)
	{
		delete session->second;
		session->second = NULL;
	}
	_sessions.erase(session);
}

void	Session::_deleteSession(const std::string& sessionId)
{
	mapOfSessions::iterator session;

	session = _sessions.find(sessionId);
	if (session == _sessions.end())
		return ;
	if (session->second)
	{
		delete session->second;
		session->second = NULL;
	}
	_sessions.erase(sessionId);
}

/******************************************************************************/
/*                                   GETTER                                   */
/******************************************************************************/

Session::mapOfSessions&		Session::getSessions()
{
	return (_sessions);
}

const Session::mapOfSessions&	Session::getSessions() const
{
	return (_sessions);
}
