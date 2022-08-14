#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <map>

class	IBlock
{
	protected :
		int										_port;
		std::string								_root;
		std::vector<std::string>				_indexList;
		bool									_autoindex;
		size_t									_bodyLimit;
		std::map<std::string, std::string>		_cgi;
		std::map<int, std::string>				_errorPage;
		std::string								_redirection;
		std::string								_uploadPath;

	public :
};

class	IServer
{
	private :
		std::string								_name;
		std::string								_host;
		int										_port;
		std::map<std::string, ILocation *>		_locations;

	public :

};

class	ILocation
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