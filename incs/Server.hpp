#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <map>

typedef enum e_method
{
	GET = 0,
	POST,
	DELETE,
}	t_method;

class	IBlock
{
	protected :
		std::vector<std::string>				_serverName;
		int										_listenPort;
		std::string								_listenHost;
		std::string								_root;
		std::vector<std::string>				_indexList;
		bool									_autoindex;
		size_t									_bodyLimit;
		std::map<std::string, std::string>		_cgi;
		std::map<int, std::string>				_errorPage;
		std::string								_redirectUri;
		int										_redirectCode;
		std::string								_uploadPath;

	public :
};

class	ServerRules : public IBlock
{
	private :
		std::map<std::string, LocationRules *>	_locations;

	public :

};

class	LocationRules : public IBlock
{
	public :

	private :
};

class	Server
{
	public :

	private :

};

#endif