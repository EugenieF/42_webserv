# include "Server.hpp"

Server::Server():
	_listeningPort(8000),
	_listeningHost("0.0.0.0")
{}

Server::~Server() {}

void	Server::setServerName(const std::string &name)
{
	_serverName.insert(_serverName.end(), name);
}

void	Server::setListeningPort(int port)
{
	_listeningPort = port;
}

int		Server::getListeningPort() const
{
	return (_listeningPort);
}

void	Server::setListeningHost(const std::string &host)
{
	_listeningHost = host;
}

const std::string&	Server::getListeningHost() const
{
	return (_listeningHost);
}
