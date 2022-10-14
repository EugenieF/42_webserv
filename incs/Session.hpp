#ifndef SESSION_HPP
# define SESSION_HPP

# define SESSION_ID_LENGTH 16

# include "utils.hpp"
# include "Cookie.hpp"

/******************************************************************************/
/*                               CLASS SESSION                                */
/******************************************************************************/

class Session
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef	std::map<std::string, Cookie*>	mapOfSessions;

	private:
	/**********************     MEMBER VARIABLES     ********************/
		mapOfSessions		_sessions;

	public: 
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Session();
		~Session();

						/*-------     Lookup   -------*/
		Cookie*				lookupSession(const Cookie& requestCookies);

						/*-------     Cleanup  -------*/
		void				deleteSessions(const std::string& sessionId);

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*--------     Setup   ------*/
		void				_newSession();
		std::string			_generateSessionId();
		std::string			_generateRandomString(int length);

						/*-------     Cleanup   -------*/
		void				_deleteSession(mapOfSessions::const_iterator session);
		void				_deleteSession(const std::string& sessionId);
};

#endif
