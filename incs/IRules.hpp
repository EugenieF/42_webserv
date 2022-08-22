#ifndef IBLOCK_HPP
# define IBLOCK_HPP

# include <string>
# include <vector>
# include <map>
# include <string>
# include <iostream>

# define DEFAULT_CLIENT_BODY_LIMIT 1000000
# define DEFAULT_ROOT ""

typedef enum e_method
{
	GET	= 0,
	POST,
	DELETE,
	ALLOWED_METHODS_COUNT,
}	t_method;

typedef enum e_context
{
	SERVER		= 0,
	LOCATION	= 1,
}	t_context;

class	IRules
{
	public:
		typedef	std::vector<std::string>		listOfStrings;

	protected :
		std::string								_root;
		listOfStrings							_indexes;
		bool									_autoindex;
		size_t									_clientBodyLimit;
		std::string								_cgiExt;
		std::string								_cgiPath;
		std::map<int, std::string>				_errorPages;
		std::string								_redirectUri;
		int										_redirectCode;
		bool									_methods[ALLOWED_METHODS_COUNT];
		std::string								_allowedMethods[ALLOWED_METHODS_COUNT];
		std::string								_uploadPath;

	public :
		IRules();
		~IRules();

		void						setRoot(const std::string &root);
		const std::string&			getRoot() const;

		void						setIndex(const std::string &index);
		listOfStrings&				getIndexes();

		void						setAutoindex(bool value);
		bool						getAutoindex() const;

		void						setClientBodyLimit(size_t maxSize);
		size_t						getClientBodyLimit() const;

		void						setCgi(const std::string &extension, const std::string &path);
		const std::string&			getCgiExt() const;
		const std::string&			getCgiPath() const;

		void						setErrorPage(int code, const std::string &page);
		int							getErrorCode();
		const std::string&			getErrorPage();

		void						setRedirection(int code, const std::string &uri);
		int							getRedirectCode();
		const std::string&			getRedirectUri();

		void						setUploadPath(const std::string &path);
		const std::string&			getUploadPath() const;

		void						setMethod(t_method method);
		bool						isAllowedMethod(std::string str);

		void						displayListOfStrings(listOfStrings list);
		void						displayRulesParams(t_context context);

	private:
		void						initAllowedMethods();

};

#endif
