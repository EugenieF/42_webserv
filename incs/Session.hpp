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
		mapOfSessions			_sessions;

	public: 
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Session();
		Session(const Session& other);
		Session&				operator=(const Session& other);
		~Session();

						/*-------     Lookup   -------*/
		Cookie*					lookupSession(const Cookie& requestCookies);

						/*-------     Cleanup  -------*/
		void					deleteSessions();

						/*--------    Getter   -------*/
		mapOfSessions&			getSessions();
		const mapOfSessions&	getSessions() const;

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*--------     Setup   ------*/
		Cookie*					_newSession();
		std::string				_generateSessionId();
		std::string				_generateRandomString(size_t length);

						/*-------     Cleanup   -------*/
		void					_deleteSession(mapOfSessions::iterator session);
		void					_deleteSession(const std::string& sessionId);
};

#endif
