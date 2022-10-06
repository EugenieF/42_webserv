/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:12 by etran             #+#    #+#             */
/*   Updated: 2022/10/06 14:50:41 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "Server.hpp"

Server::Server(Server::listOfServers servers, char* const* env) :
	_env(env),
	_servers(servers) {
		_createSocketList();
		_epoll.setSocketList(_socketList);
		// _displayServer();
		DEBUG("Server Block constructor");
	}

Server::~Server() {
	_clear();
	DEBUG("Server Block destructor");
}

/* PUBLIC =================================================================== */

// Server management -----------------------------

void Server::launchServer() {
	_epoll.startMonitoring(_env);
}

// Getter ----------------------------------------

// int Server::getPort() const {
// 	return (ntohs(_addr.sin_port));
// }

// const std::string& Server::getHost() const {
// 	return (_ip);
// }

int Server::getEpoll() const {
	return (_epoll.getFd());
}

// int Server::getSocket() const {
// 	return (_socket.getFd());
// }

// const struct sockaddr_in& Server::getAddr() const {
// 	return (_addr);
// }

/* PRIVATE ================================================================== */

// Debug -----------------------------------------

// void Server::_displayServer(Block* server) const {
// 	std::cout	<< "== Connection infos ==" << NL
// 				<< "fd: " << getSocket() << NL
// 				<< "ip: " << getHost() << NL
// 				<< "port: " << getPort() << NL;
// }

TcpSocket*	Server::_createSocket(int port, const std::string& ipAddress)
{
	struct sockaddr_in	addr;
	
	TcpSocket *newSocket = new TcpSocket(socket(PF_INET, SOCK_STREAM, 0));
	newSocket->setToReusable();
	memset(&addr, 0, sizeof(addr)); 
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ipAddress.c_str());

	if (bind(newSocket->getFd(), reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) < 0)
		throw std::runtime_error("bind error");
	newSocket->unlockSocket();
	newSocket->listenSocket();
	return (newSocket);
}

void	Server::_createSocketList()
{
	listOfServers::const_iterator	currentServer;
	TcpSocket*						newSocket;

	for (currentServer = _servers.begin(); currentServer != _servers.end(); currentServer++)
	{
		newSocket = _createSocket((*currentServer)->getPort(), (*currentServer)->getHost());
		_socketList.insert(std::make_pair(newSocket, *currentServer));
	}
}

void	Server::_clear()
{
	listOfSockets::const_iterator	socket;

	for (socket = _socketList.begin(); socket != _socketList.end(); socket++)
		delete socket->first;
}
