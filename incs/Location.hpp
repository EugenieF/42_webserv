#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "IRules.hpp"

typedef enum e_method
{
	GET	= 0,
	POST,
	DELETE,
	ALLOWED_METHODS_COUNT,
}	t_method;

class	Location : public IRules
{
	public :
		Location();
		~Location();

		void				setUploadPath(const std::string &path);
		const std::string	&getUploadPath() const;

		void				setAllowedMethod(t_method method);
		bool				isAllowedMethod(t_method method);

	private :
		bool				_allowedMethods[ALLOWED_METHODS_COUNT];
		std::string			_uploadPath;
};


#endif