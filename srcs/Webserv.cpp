# include "debug.hpp"
#include "Webserv.hpp"

/******************************************************************************/
/*                                  MAIN                                      */
/******************************************************************************/

Webserv::Webserv() {}

Webserv::Webserv(std::string configFile, char* const* env):
	_parser(configFile),
	_env(env) {
	(void)_env;
	DEBUG("Getting servers ...");
	_setupServerMap(getConfigServers());
	run();	
}

Webserv::Webserv(const Webserv& other)
{
	*this = other;
}

Webserv::~Webserv() {
	serverMap::iterator     it = _servers.begin();

//     for (it = _servers.begin(); it != _servers.end(); it++) {
	while (it != _servers.end()) {
		delete it->first;
		_servers.erase(it);
		it = _servers.begin();
	}
}

Webserv&	Webserv::operator=(const Webserv& other)
{
	if (this != &other)
	{
		_parser = other.getParser();
		_env = other.getEnv();
		_servers = other.getServers();
		_epoll = other.getEpoll();
	}
	return (*this);
}

/******************************************************************************/
/*                                  SETUP                                     */
/******************************************************************************/

void	Webserv::_setupServerMap(Webserv::listOfServers configServers) {
	listOfServers::const_iterator	ite;

	for (ite = configServers.begin(); ite != configServers.end(); ite++)
		_servers.insert(std::pair<Server*, Block*>(new Server(*ite), *ite));
}

void	Webserv::parse(std::string configFile) {
	_parser.parseFile(configFile);
	_setupServerMap(getConfigServers());
}

void	Webserv::run() {
	// Launching servers
	DEBUG("Launching ...");
	_epoll.startMonitoring(_servers, _env);
	DEBUG("Webserv created");
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

Parser	Webserv::getParser() const
{
	return (_parser);
}

Webserv::serverMap	Webserv::getServers() const
{
	return (_servers);
}

EpollInstance	Webserv::getEpoll() const
{
	return (_epoll);
}

char* const*	Webserv::getEnv() const
{
	return (_env);
}

Parser::listOfServers	Webserv::getConfigServers()
{
	return (_parser.getServers());
}

int		Webserv::getNbOfServers()
{
	int										count;
	Parser::listOfServers::const_iterator	ite;

	count = 0;
	for (ite = getConfigServers().begin(); ite != getConfigServers().end(); ite++){
		count++;
		// std::cout << "server nb is " << count << NL;
	}
	return (count);
}

/******************************************************************************/
/*                                 DISPLAY                                    */
/******************************************************************************/

void	Webserv::displayServers()
{
	_parser.displayServersParams();
}
