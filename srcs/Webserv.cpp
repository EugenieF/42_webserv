#include "Webserv.hpp"

Webserv::Webserv(): _parser() {}

Webserv::Webserv(std::string configFile): _parser(configFile) {}

Webserv::Webserv(const Webserv& other): _parser(other.getParser())
{
	*this = other;
}

Webserv::~Webserv() {}

void	Webserv::displayServers()
{
	_parser.displayServersParams();
}

Webserv&	Webserv::operator=(const Webserv& other)
{
	if (this != &other)
	{
		_parser = other.getParser();
	}
	return (*this);
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

Parser	Webserv::getParser() const
{
	return (_parser);
}
