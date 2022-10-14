#ifndef COOKIE_HPP
# define COOKIE_HPP

# include "utils.hpp"

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

	public:
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Cookie();
		Cookie(const Cookie& other);
		~Cookie();
		void				fillCookies(const Cookie& other);

						/*-------    Setter   -------*/
		void				setCookie(const std::string& name, const std::string& value);
		void				setCookies(std::string header);
		std::string			setCookieHeader();

						/*-------    Getter   -------*/
		mapOfCookies		getCookies() const;
		std::string			getCookie(const std::string& name);
		std::string			getSessionId() const;

						/*-------     Utils   -------*/
		bool				isEmpty();
		bool				isSet(const std::string& name);
		void				display();
};

#endif
