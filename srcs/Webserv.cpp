# include "debug.hpp"
#include "Webserv.hpp"

/******************************************************************************/
/*                                  MAIN                                      */
/******************************************************************************/

Webserv::Webserv() {}

Webserv::Webserv(std::string configFile, char* const* env):
	_parser(configFile) {
	DEBUG("Launching webserv");
	_setupServerMap(getConfigServers(), env);
	setupSignal(INIT_SIGNAL);
	run();	
}

// Webserv::Webserv(const Webserv& other)
// {
// 	*this = other;
// }

Webserv::~Webserv() {
	serverMap::iterator     it = _servers.begin();

	while (it != _servers.end()) {
		Server* tmp = it->first;
		_servers.erase(it);
		delete tmp;
		it = _servers.begin();
	}
	setupSignal(RESET_SIGNAL);
	DEBUG("Deleted webserv");
}

// Webserv&	Webserv::operator=(const Webserv& other)
// {
// 	if (this != &other)
// 	{
// 		_parser = other.getParser();
// 		_servers = other.getServers();
// 		_epoll = other.getEpoll();
// 	}
// 	return (*this);
// }

/******************************************************************************/
/*                                  SETUP                                     */
/******************************************************************************/

void	Webserv::_setupServerMap(const listOfServers& configServers,
	char* const* env) {
	listOfServers::const_iterator   ite;

	for (ite = configServers.begin(); ite != configServers.end(); ite++)
		_servers.insert(std::pair<Server*,
			Block*>(new Server(*ite, env), *ite));
}

// void	Webserv::parse(std::string configFile) {
// 	_parser.parseFile(configFile);
// 	_setupServerMap(getConfigServers());
// }

void	Webserv::run() {
	displayMsg(" 🏁 Server is launching...", LIGHT_YELLOW);
	_epoll.startMonitoring(_servers);
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Parser	Webserv::getParser() const
{
	return (_parser);
}

const Webserv::serverMap&	Webserv::getServers() const
{
	return (_servers);
}

const EpollInstance&	Webserv::getEpoll() const
{
	return (_epoll);
}

const Parser::listOfServers&	Webserv::getConfigServers() const
{
	return (_parser.getServers());
}

int		Webserv::getNbOfServers() const
{
	int										count;
	Parser::listOfServers::const_iterator	ite;

	count = 0;
	for (ite = getConfigServers().begin(); ite != getConfigServers().end(); ite++) {
		count++;
	}
	return (count);
}

/******************************************************************************/
/*                                 DISPLAY                                    */
/******************************************************************************/

void	Webserv::displayServers() const
{
	_parser.displayServersParams();
}
