#ifndef BLOCK_HPP
# define BLOCK_HPP

# include <vector>
# include <map>
# include <string>
# include <unistd.h>
# include <iostream>
# include <utility>

# include "Colors.hpp"
# include "utils.hpp"
# include "HttpMethod.hpp"

extern HttpMethod	g_httpMethod;

# define DEFAULT_CLIENT_BODY_LIMIT 1000000
# define DEFAULT_ROOT ""

/******************************************************************************/
/*                                 ENUMS                                      */
/******************************************************************************/

typedef enum e_context
{
	NONE		= 0,
	SERVER		= 1,
	LOCATION	= 2,
}	t_context;

/******************************************************************************/
/*                                CLASS BLOCK                                 */
/******************************************************************************/

class	Block
{
	public :
	/***********************      MEMBER TYPES      *********************/
		typedef Block*								blockPtr;
		typedef	std::vector<std::string>			listOfStrings;
		typedef std::map<std::string, blockPtr>		listOfLocations;
		typedef std::map<int, std::string>			listOfErrorPages;
		typedef std::vector<Block*>					listOfServers;

	private :
	/**********************     MEMBER VARIABLES     ********************/

					/*-----   Server block only   -----*/
		t_context							_context;
		listOfStrings						_serverNames;
		int									_port;
		std::string							_host;
		listOfServers						_virtualHosts;

					/*---- Server / Location block ----*/
		std::string							_root;
		listOfStrings						_indexes;
		bool								_autoindex;
		size_t								_clientBodyLimit;
		std::string							_cgiExt;
		std::string							_cgiPath;
		listOfErrorPages					_errorPages;
		int									_redirectCode;
		std::string							_redirectUri;

					/*-----  Location block only  -----*/
		listOfLocations						_locations;
		listOfLocations::const_iterator		_currentLocation;
		bool								_methods[ALLOWED_METHODS_COUNT];
		std::string							_uploadPath;


	public :
	/*********************  PUBLIC MEMBER FUNCTIONS  *******************/

						/*-------     Main    -------*/
		Block();
		Block(const Block& other);
		~Block();
		Block&								operator=(const Block& other);
		void								completeLocationDirectives(const Block& server);
		Block*								getMatchingBlock(const std::string& path, std::string* locationPath);

						/*-------  Server_name  ------*/
		void								setName(const std::string& name);
		listOfStrings						getServerNames() const;	

						/*-------    Listen    -------*/
		void								setPort(int port);
		int									getPort() const;
		void								setHost(const std::string& host);
		const std::string&					getHost() const;

						/*-------     Root     -------*/
		void								setRoot(const std::string& root);
		const std::string&					getRoot() const;

						/*-------     Index    -------*/
		void								setIndex(const std::string& index);
		listOfStrings						getIndexes() const;

						/*-------   Autoindex  -------*/
		void								setAutoindex(bool value);
		bool								getAutoindex() const;

						/*--- Client_max_body_size ---*/
		void								setClientBodyLimit(size_t size);
		unsigned long						getClientBodyLimit() const;

						/*-------      CGI     -------*/
		void								setCgiExt(const std::string& extension);
		void								setCgiPath(const std::string& path);
		const std::string&					getCgiExt() const;
		const std::string&					getCgiPath() const;
		bool								cgiDirective();

						/*-------  Error_page  -------*/
		void								setErrorPage(int code, const std::string& page);
		listOfErrorPages					getErrorPages();
		std::string							getErrorPage(int statusCode);

						/*-------    Redirect  -------*/
		void								setRedirection(int code, const std::string& uri);
		int									getRedirectCode();
		const std::string&					getRedirectUri() const;
		int									getRedirectCode() const;
		bool								redirectDirective();

						/*-------  Upload_path  -------*/
		void								setUploadPath(const std::string& path);
		const std::string&					getUploadPath() const;
		bool								uploadPathDirective();

						/*------- Allowed_method ------*/
		void								setMethod(t_method method);
		void								setMethod(const std::string& str);
		bool								isAllowedMethod(t_method method);

						/*-------    Location   ------*/
		bool								insertLocation(const std::string& path, blockPtr newLocation);
		int									getNbOfLocations() const;
		blockPtr							getCurrentLocation();
		listOfLocations						getLocations() const;

						/*-------    Context    ------*/
		void								setContext(t_context context);
		t_context							getContext() const;

						/*-------     Utils    -------*/
		bool								isServerBlock();
		bool								isLocationBlock();

						/*-------    Display    ------*/
		void								displayListOfStrings(listOfStrings list);
		void								displayBlockDirectives(t_context context);
		void								displayParams(int num);
		void								displayServerNames();

						/*------ Virtual hosts -----*/
		void								setVirtualHost(blockPtr server);
		listOfServers						getVirtualHosts() const;
		bool								hasVirtualHosts();

		bool								operator==(Block const& otherServer);

	private:
	/*********************  PRIVATE MEMBER FUNCTIONS  *******************/

						/*-------     Utils    -------*/
		void								_deleteLocations();
};


#endif
