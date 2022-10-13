#ifndef COOKIE_HPP
# define COOKIE_HPP

# include "utils.hpp"

class Cookie
{
	public:
		typedef std::map<std::string, std::string> mapOfCookies;

	private:
		mapOfCookies	_cookies;

	public:
		Cookie();
		Cookie(const Cookie& other);
		~Cookie();

		void			setCookie(const std::string& name, const std::string& value);
		void			setCookies(std::string header);
		mapOfCookies	getCookies() const;
		std::string		getCookie(const std::string& name);
		bool			isEmpty();
		bool			isSet(const std::string& name);
		void			display();
		std::string		setCookieHeader();
};

#endif
