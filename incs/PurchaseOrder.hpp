#ifndef COOKIE_HPP
# define COOKIE_HPP

# include "utils.hpp"

# define SESSION_TIMEOUT 1000

/******************************************************************************/
/*                                CLASS COOKIE                                */
/******************************************************************************/

class Cookie
{
	public:
	/***********************      MEMBER TYPES      *********************/
		typedef std::map<std::string, std::string> mapOfCookies;

	private:
	/**********************     MEMBER VARIABLES     ********************/
		std::string			_sessionId;
		mapOfCookies		_cookies;
		size_t				_time;

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Cookie();
		Cookie(const std::string& sessionId);
		Cookie(const Cookie& other);
		Cookie&				operator=(const Cookie& other);
		~Cookie();
		void				fillCookies(const Cookie& other);

						/*-------    Setter   -------*/
		void				setCookie(const std::string& name, const std::string& value);
		void				setCookies(std::string header);
		std::string			setCookieHeader();

						/*-------    Getter   -------*/
		mapOfCookies&		getCookies();
		const mapOfCookies&	getCookies() const;
		std::string			getCookie(const std::string& name);
		std::string			getSessionId() const;

						/*-------     Size   -------*/
		size_t				size() const;
		bool				isEmpty();
		void				checkSizeCookie(const std::string& name, const std::string& value);

						/*-------     Time   -------*/
		bool				sessionIsAlive();
		void				updateTime();

						/*-------     Utils   -------*/
		bool				isSet(const std::string& name);
		void				display() const;


		std::string&		operator[](const std::string& name);
};

#endif
