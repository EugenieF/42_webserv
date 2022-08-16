#ifndef IBLOCK_HPP
# define IBLOCK_HPP

# include <string>
# include <vector>
# include <map>

# define DEFAULT_CLIENT_BODY_LIMIT 1000000
# define DEFAULT_ROOT ""

class	IRules
{
	protected :
		std::string								_root;
		std::vector<std::string>				_indexes;
		bool									_autoindex;
		size_t									_clientBodyLimit;
		std::string								_cgiExtension;
		std::string								_cgiPath;
		std::map<int, std::string>				_errorPages;
		std::string								_redirectUri;
		int										_redirectCode;

	public :
		IRules();
		~IRules();

		void				setRoot(const std::string &root);
		const std::string&	getRoot() const;

		void				setIndex(const std::string &index);

		void				setAutoindex(bool value);
		bool				getAutoindex() const;

		void				setClientBodyLimit(size_t maxSize);
		size_t				getClientBodyLimit() const;

		void				setCgi(const std::string &extension, const std::string &path);

		void				setErrorPage(int code, const std::string &page);

		void				setRedirection(const std::string &uri, int code);
};

#endif
