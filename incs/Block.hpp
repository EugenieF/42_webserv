#ifndef BLOCK_HPP
# define BLOCK_HPP

# include <vector>
# include <map>
# include <string>
# include <unistd.h>
# include <iostream>
# include <utility>

# include "Colors.hpp"

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
		typedef std::map<int, std::string>		listOfErrorPages;

	private :
		t_context								_context;
		listOfStrings							_serverNames;
		int										_port;
		std::string								_host;

		std::string								_root;
		listOfStrings							_indexes;
		bool									_autoindex;
		size_t									_clientBodyLimit;
		std::string								_cgiExt;
		std::string								_cgiPath;
		listOfErrorPages						_errorPages;
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
		void									setContext(t_context context);

		/*******************************    SERVER_NAME DIRECTIVE    ****************************/

		void									setName(const std::string &name);

		/*******************************       LISTEN DIRECTIVE       *****************************/

		void									setPort(int port);
		int										getPort() const;

		void									setHost(const std::string& host);
		const std::string&						getHost() const;

		/*******************************       ROOT DIRECTIVE       *****************************/
	
		void									setRoot(const std::string& root);
		const std::string&						getRoot() const;

		/*******************************      INDEX DIRECTIVE       *****************************/

		void									setIndex(const std::string& index);
		listOfStrings&							getIndexes();

		/*******************************     AUTOINDEX DIRECTIVE    *****************************/

		void									setAutoindex(bool value);
		bool									getAutoindex() const;

		void									setClientBodyLimit(size_t size);
		unsigned long							getClientBodyLimit() const;

		/*******************************        CGI DIRECTIVE       *****************************/

		void									setCgiExt(const std::string& extension);
		void									setCgiPath(const std::string& path);
		const std::string&						getCgiExt() const;
		const std::string&						getCgiPath() const;

		/*******************************    ERROR_PAGE DIRECTIVE    *****************************/

		void									setErrorPage(int code, const std::string& page);
		listOfErrorPages						getErrorPages();

		/*******************************     REDIRECT DIRECTIVE     *****************************/

		void									setRedirection(int code, const std::string& uri);
		int										getRedirectCode();
		const std::string&						getRedirectUri();

		/*******************************   UPLOAD_PATH DIRECTIVE   ******************************/

		void									setUploadPath(const std::string& path);
		const std::string&						getUploadPath() const;

		/******************************   ALLOWED_METHOD DIRECTIVE  *****************************/

		void									setMethod(t_method method);
		bool									isAllowedMethod(const std::string& str);


		void									displayListOfStrings(listOfStrings list);
		void									displayBlockDirectives(t_context context);

		bool									insertLocation(const std::string& path, blockPtr newLocation);

		void									displayParams(int num);
		void									displayServerNames();

		blockPtr								getCurrentLocation();

	private:
		void									initAllowedMethods();
		void									deleteLocations();

};

#endif
