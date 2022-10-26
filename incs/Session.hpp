#ifndef SESSION_HPP
# define SESSION_HPP

# define SESSION_ID_LENGTH 16
# define SESSION_TIMEOUT 1000

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
						
						/*-------     Time   -------*/
		bool					sessionIsAlive();
		void					updateTime();

						/*-------   Cookies  -------*/
		void					setCookies(std::string header);
		void					fillCookies(const listOfCookies& other);
		void					addCookie(const std::string& name, const std::string& value);

						/*--------  Purchase  -------*/
		void					addPurchase(const std::string& name, const std::string& hamster, const std::string& color);
		void					deletePurchase(listOfPurchases::iterator ite);
};

class SessionHandler
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef Session::listOfCookies		listOfCookies;
		typedef Session::listOfPurchases	listOfPurchases;
		typedef std::vector<Session>		listOfSessions;

	private:
	/**********************     MEMBER VARIABLES     ********************/
		listOfSessions						_sessions;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

		SessionHandler();
		~SessionHandler();

		listOfCookies						getSessionCookies(std::string sessionId);
		listOfPurchases						getSessionOrder(std::string sessionId);

								/*-------     Lookup   -------*/
		std::vector<Session>::const_iterator	findSession(std::string sessionId);
		listOfCookies							lookupSession(const Cookie& requestCookies);

								/*-------     Getter   -------*/
		listOfSessions&						getSessions();
		const listOfSessions&					getSessions() const;

		// Cookie*							fillSessionCookies(const Cookie& requestCookies);
		// Order*							fillSessionOrder(const Order& responseOrder);

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

								/*--------     Setup   ------*/
		Session&							_newSession();
		std::string							_generateSessionId();
		std::string							_generateRandomString(size_t length);
};

#endif
