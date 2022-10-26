#ifndef SESSION_HPP
# define SESSION_HPP

# define SESSION_ID_LENGTH 16

# include "utils.hpp"
# include "Cookie.hpp"
# include "PurchaseOrder.hpp

# include <vector>

/******************************************************************************/
/*                               CLASS SESSION                                */
/******************************************************************************/

class Session
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef std::vector<class Purchase>			listOfPurchases;

	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string				_id;
		Cookie*					_cookies;
		listOfPurchases			_order;

	public: 
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Session(const std::string& id);
		Session(const Session& other);
		Session&				operator=(const Session& other);
		~Session();

						/*--------    Getter   -------*/
		std::string				getId() const;
		Cookies*				getCookies();
		listOfPurchases&		getOrder();
		const listOfPurchases&	getOrder() const;

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/
};

class SessionHandler
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef Session::listOfPurchases	listOfPurchases;

	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::vector<Session>				_sessions;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

		SessionHandler();
		~SessionHandler();

		Cookies*							getSessionCookies(std::string sessionId);
		listOfPurchases						getSessionOrder(std::string sessionId);

								/*-------     Lookup   -------*/
		std::vector<Session>::const_iterator	findSession(std::string sessionId);
		Cookies*							lookupSession(const Cookie& requestCookies);

								/*-------     Getter   -------*/
		std::vector<Session>&				getSessions();
		const std::vector<Session>&			getSessions() const;

		// Cookie*							fillSessionCookies(const Cookie& requestCookies);
		// Order*							fillSessionOrder(const Order& responseOrder);

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

								/*--------     Setup   ------*/
		Cookies*							_newSession();
		std::string							_generateSessionId();
		std::string							_generateRandomString(size_t length);
};

#endif
