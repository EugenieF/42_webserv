#include "Session.hpp"

/******************************************************************************/
/*                                   MAIN                                     */
/******************************************************************************/

Session::Session() {}

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
	mapOfSession::const_iterator	ite;
	Cookie*							cookies;

	sessionId = requestCookies.getSessionId();
	if (sessionId == "")
		cookies = _newSession();
	else
	{
		ite = _sessions.find(sessionId);
		if (ite == _sessions.end())
			cookies = _newSession();
		else
			cookies = ite->second;
	}
	cookies->fillCookies(requestCookies);
	return (cookies);
}

/******************************************************************************/
/*                                   SETUP                                    */
/******************************************************************************/

Cookies*	Session::_newSession()
{
	std::string	newId;
	Cookie*		newCookie;

	newId = _generateSessionId();
	newCookie = new Cookie;
	newCookie->setCookie("SID", newId);
	_sessions.insert(newId, newCookie);
	return (newCookie);
}

std::string 	Session::_generateRandomString(int length)
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

	sessionId = generateRandomString(SESSION_ID_LENGTH);
	while (_sessions.find(sessionId) != _sessions.end())
		sessionId = generateRandomString(SESSION_ID_LENGTH);
	return (sessionId);
}

/******************************************************************************/
/*                                  CLEANUP                                   */
/******************************************************************************/

void	Session::deleteSessions(const std::string& sessionId)
{
	mapOfSessions::const_iterator	session;

	for (session = _sessions.begin(); session != _sessions.end(); session++)
		_deleteSession(session);
}

void	Session::_deleteSession(Session::mapOfSessions::const_iterator session)
{
	if (session == _sessions.end())
		return ;
	if (session->second)
	{
		delete session->second;
		session->second = NULL;
	}
	_sessions.erase(sessionId);
}

void	Session::_deleteSession(const std::string& sessionId)
{
	mapOfSessions::const_iterator session;

	session = _sessions.find(sessionId);
	if (session == _sessions.end())
		return ;
	if (session->second)
	{
		delete session->second
		session->second = NULL;
	}
	_sessions.erase(sessionId);
}
