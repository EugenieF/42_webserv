# include "Server.hpp"

Server::Server():
	_port(8000),
	_host("0.0.0.0")
{
	if (!getuid())
		setPort(80);
}

Server::~Server()
{
	deleteLocations();
}

bool	Server::insertLocation(std::string path)
{
	std::pair<listOfLocations::const_iterator, bool>	pair;
	
	pair = _locations.insert(std::make_pair<std::string, Location *>(path, new Location()));
	_currentLocation = pair.first;
	return (pair.second);
}

void	Server::setName(const std::string &name)
{
	_names.insert(_names.end(), name);
}

void	Server::setPort(int port)
{
	_port = port;
}

int		Server::getPort() const
{
	return (_port);
}

void	Server::setHost(const std::string &host)
{
	_host = host;
}

const std::string&	Server::getHost() const
{
	return (_host);
}

void	Server::deleteLocations()
{
	for (_currentLocation = _locations.begin(); _currentLocation != _locations.end(); _currentLocation++)
	{
		std::cout << ORANGE << " xxx Delete a location xxx" << RESET << std::endl;
		delete (_currentLocation->second);
	}
}

Location	*Server::getCurrentLocation()
{
	return (_currentLocation->second);
}

void	Server::displayParams(int num)
{
	listOfLocations::const_iterator	ite;

	std::cout << std::endl << " SERVER " << num << ": " << std::endl;
	std::cout << "  ‣ Names: ";
	displayListOfStrings(_names);
	std::cout << "  ‣ Port: " << getHost() << ":" << getPort() << std::endl;
	displayRulesParams(SERVER);
	for (ite = _locations.begin(); ite != _locations.end(); ite++)
	{
		std::cout << std::endl << "     Location " << ite->first << ": " << std::endl;
		ite->second->displayRulesParams(LOCATION);
	}
}
