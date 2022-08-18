# include "Server.hpp"

Server::Server():
	_port(8000),
	_host("0.0.0.0")
{
	if (!getuid())
		setPort(80);
}

Server::~Server() {}

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
