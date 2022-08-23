#ifndef BLOCK_HPP
# define BLOCK_HPP

# include <vector>
# include <map>
# include <string>
# include <unistd.h>
# include <iostream>
# include <utility>

# include "webserv.hpp"

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
	NONE		= 0,
	SERVER		= 1,
	LOCATION	= 2,
}	t_context;

class	Block
{
	public:
		typedef Block*							blockPtr;
		typedef	std::vector<std::string>		listOfStrings;
		typedef std::map<std::string, blockPtr>	listOfLocations;

	private :
		t_context								_context;
		listOfStrings							_serverNames;
		int										_port;
		std::string								_host;

		std::string								_root;
		listOfStrings							_indexes;
		bool									_autoindex;
		unsigned long							_clientBodyLimit;
		std::string								_cgiExt;
		std::string								_cgiPath;
		std::map<int, std::string>				_errorPages;
		int										_redirectCode;
		std::string								_redirectUri;

		listOfLocations							_locations;
		listOfLocations::const_iterator			_currentLocation;
		bool									_methods[ALLOWED_METHODS_COUNT];
		std::string								_allowedMethods[ALLOWED_METHODS_COUNT];
		std::string								_uploadPath;


	public :
		Block();
		~Block();

		bool									isServerBlock();
		bool									isLocationBlock();

		void									setRoot(const std::string &root);
		const std::string&						getRoot() const;

		void									setIndex(const std::string &index);
		listOfStrings&							getIndexes();

		void									setAutoindex(bool value);
		bool									getAutoindex() const;

		void									setClientBodyLimit(unsigned long maxSize);
		unsigned long							getClientBodyLimit() const;

		void									setCgi(const std::string &extension, const std::string &path);
		const std::string&						getCgiExt() const;
		const std::string&						getCgiPath() const;

		void									setErrorPage(int code, const std::string &page);
		int										getErrorCode();
		const std::string&						getErrorPage();

		void									setRedirection(int code, const std::string &uri);
		int										getRedirectCode();
		const std::string&						getRedirectUri();

		void									setUploadPath(const std::string &path);
		const std::string&						getUploadPath() const;

		void									setMethod(t_method method);
		bool									isAllowedMethod(std::string str);

		void									displayListOfStrings(listOfStrings list);
		void									displayBlockDirectives(t_context context);

		void									setName(const std::string &name);

		void									setPort(int port);
		int										getPort() const;

		void									setHost(const std::string &host);
		const std::string&						getHost() const;

		bool									insertLocation(std::string path, blockPtr newLocation);

		void									displayParams(int num);
		void									displayServerNames();

		blockPtr								getCurrentLocation();

	private:
		void									initAllowedMethods();
		void									deleteLocations();

};

#endif
