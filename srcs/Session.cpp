#include "Session.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Session::Session()
{
	struct timeval	time;

    gettimeofday(&time,NULL);
	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

Session::~Session()
{
	deleteSessions();
}

/******************************************************************************/
/*                                   LOOKUP                                   */
/******************************************************************************/

Cookie*	Session::lookupSession(const Cookie& requestCookies)
{
	std::string						sessionId;
	mapOfSessions::const_iterator	ite;
	Cookie*							cookies;

	requestCookies.display();
	sessionId = requestCookies.getSessionId();
	std::cout << RED << "sessionId = '" << sessionId << "'" << RESET << std::endl;
	if (sessionId == "")
		cookies = _newSession();
	else
	{
		ite = _sessions.find(sessionId);
		if (ite == _sessions.end())
		{
			std::cout << RED << " Not found" << std::endl;
			cookies = _newSession();
		}
		else
		{
			std::cout << RED << "***** FOUND ID *****" << std::endl;
			cookies = ite->second;
		}
	}
	cookies->fillCookies(requestCookies);
	cookies->display();
	return (cookies);
}

/******************************************************************************/
/*                                   SETUP                                    */
/******************************************************************************/

Cookie*	Session::_newSession()
{
	std::string	newId;
	Cookie*		newCookie;

	newId = _generateSessionId();
	newCookie = new Cookie;
	newCookie->setCookie("SID", newId);
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
	mapOfSessions::iterator	session;

	for (session = _sessions.begin(); session != _sessions.end(); session++)
		_deleteSession(session);
}

void	Session::_deleteSession(Session::mapOfSessions::iterator session)
{
	if (session == _sessions.end())
		return ;
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
