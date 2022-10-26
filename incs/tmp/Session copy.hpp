#ifndef SESSION_HPP
# define SESSION_HPP

# define SESSION_ID_LENGTH 16

# include "utils.hpp"
# include "Cookie.hpp"
# include "PurchaseOrder.hpp

/******************************************************************************/
/*                               CLASS SESSION                                */
/******************************************************************************/

class Session
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef	std::map<std::string, Cookie*>	mapOfSessions;
		typedef	std::map<std::string, Order*>	mapOfOrders;

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
		Cookie*					lookupSessionCookies(const Cookie& requestCookies);
		Order*					lookupSessionOrder(const Order& responseOrder);

						/*-------     Cleanup  -------*/
		void					deleteSessions();

						/*--------    Getter   -------*/
		mapOfSessions&			getSessions();
		const mapOfSessions&	getSessions() const;

		Cookie*					getSessionCookies(const std::string& sessionId);
		Order*					getSessionOrder();

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*--------     Setup   ------*/
		// Cookie*					_newSession();
		void					_newSession();
		std::string				_generateSessionId();
		std::string				_generateRandomString(size_t length);

						/*-------     Cleanup   -------*/
		void					_deleteSession(mapOfSessions::iterator session);
		void					_deleteSession(const std::string& sessionId);

		Cookie*					_findSessionCookies(std::string sessionId);
		Order*					_findSessionOrder(std::string sessionId);
		// Cookie*					_findSession(std::string sessionId);
};

#endif
