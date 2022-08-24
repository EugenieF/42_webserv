#include "Webserv.hpp"

Webserv::Webserv(std::string configFile): _parser(configFile) {}

Webserv::~Webserv() {}

void	Webserv::displayServers()
{
	_parser.displayServersParams();
}
