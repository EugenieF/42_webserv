#include "Webserv.hpp"

Webserv::Webserv(): _parser() {}

Webserv::Webserv(std::string configFile): _parser(configFile) {}

Webserv::~Webserv() {}

void	Webserv::displayServers()
{
	_parser.displayServersParams();
}

void	Webserv::parse(std::string configFile)
{
	_parser.parseFile(configFile);
}

Parser::listOfServers	Webserv::getServers()
{
	return (_parser.getServers());
}

int		Webserv::getNbOfServers()
{
	int										count;
	Parser::listOfServers::const_iterator	ite;

	count = 0;
	for (ite = getServers().begin(); ite != getServers().end(); ite++)
		count++;
	return (count);
}
