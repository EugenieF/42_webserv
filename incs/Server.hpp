#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <map>
# include <unistd.h>

# include "IRules.hpp"
# include "Location.hpp"

class	Server : public IRules
{
	public :
		typedef std::map<std::string, Location *>	listOfLocations;

		Server();
		~Server();

		void				setName(const std::string &name);

		void				setPort(int port);
		int					getPort() const;

		void				setHost(const std::string &host);
		const std::string&	getHost() const;

	private :
		listOfLocations							_locations;
		std::vector<std::string>				_names;
		int										_port;
		std::string								_host;

};


#endif