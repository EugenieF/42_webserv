#include "Webserv.hpp"

/******************************************************************************/
/*                                  MAIN                                      */
/******************************************************************************/

Webserv::Webserv(): _parser() {}

Webserv::Webserv(std::string configFile): _parser(configFile) {}

Webserv::Webserv(const Webserv& other): _parser(other.getParser())
{
	*this = other;
}

Webserv::~Webserv() {}

Webserv&	Webserv::operator=(const Webserv& other)
{
	if (this != &other)
	{
		_parser = other.getParser();
	}
	return (*this);
}

/******************************************************************************/
/*                                  PARSER                                    */
/******************************************************************************/

void	Webserv::parse(std::string configFile)
{
	_parser.parseFile(configFile);
}

/******************************************************************************/
/*                                  GETTER                                    */
/******************************************************************************/

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

/******************************************************************************/
/*                                 DISPLAY                                    */
/******************************************************************************/

void	Webserv::displayServers()
{
	_parser.displayServersParams();
}
