#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <map>

# include "IBlock.hpp"
# include "Location.hpp"

class	Server : public IBlock
{
	public :
		typedef std::map<std::string, Location *>	listOfLocations;

		Server();
		~Server();

		void				setServerName(const std::string &name);

		void				setListeningPort(int port);
		int					getListeningPort() const;

		void				setListeningHost(const std::string &host);
		const std::string&	getListeningHost() const;

	private :
		listOfLocations							_locations;
		std::vector<std::string>				_serverName;
		int										_listeningPort;
		std::string								_listeningHost;

};


#endif