#ifndef SESSION_HPP
# define SESSION_HPP

# define SESSION_ID_LENGTH 16
# define SESSION_TIMEOUT 1000

# include "utils.hpp"
# include "Cookie.hpp"
# include "Purchase.hpp"

# include <vector>

/******************************************************************************/
/*                               CLASS SESSION                                */
/******************************************************************************/

class Session
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef std::vector<class Cookie>		listOfCookies;
		typedef std::vector<class Purchase>		listOfPurchases;

	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string				_id;
		listOfCookies			_cookies;
		listOfPurchases			_order;
		size_t					_time;

	public: 
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Session();
		Session(const std::string& id);
		Session(const Session& other);
		Session&				operator=(const Session& other);
		~Session();

						/*--------    Getter   -------*/
		std::string				getId() const;
		listOfCookies&			getCookies();
		const listOfCookies&	getCookies() const;
		listOfPurchases&		getOrder();
		const listOfPurchases&	getOrder() const;
		size_t					getTime() const;
						
						/*-------     Time   -------*/
		bool					sessionIsAlive();
		void					updateTime();

						/*-------   Cookies  -------*/
		std::string				getCookieHeader();
		void					fillCookies(const listOfCookies& other);
		void					addCookie(const std::string& name, const std::string& value);

						/*--------  Purchase  -------*/
		void					addPurchase(const std::string& name, const std::string& content);
		void					deletePurchase(listOfPurchases::iterator ite);
		void					displayCookies() const;

};

/******************************************************************************/
/*                           CLASS SESSION HANDLER                            */
/******************************************************************************/

class SessionHandler
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef Session::listOfCookies		listOfCookies;
		typedef std::vector<Session*>		listOfSessions;

	private:
	/**********************     MEMBER VARIABLES     ********************/
		listOfSessions						_sessions;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

		SessionHandler();
		~SessionHandler();

								/*-------     Lookup   -------*/
		Session*							lookupSession(const listOfCookies& requestCookies);

								/*-------     Getter   -------*/
		listOfSessions&						getSessions();
		const listOfSessions&				getSessions() const;

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

								/*--------     Setup   ------*/
		Session*							_newSession();
		std::string							_generateSessionId();
		std::string							_generateRandomString(size_t length);

								/*--------     find   ------*/
		Session*							_findSession(const std::string& sessionId);
		listOfSessions::iterator			_findSessionIte(const std::string& sessionId);
		std::string							_getCookieSID(const listOfCookies& cookies);

								/*--------   Cleanup  ------*/
		void								_deleteSessions();
		void								_deleteSession(listOfSessions::iterator ite);
};

#endif
