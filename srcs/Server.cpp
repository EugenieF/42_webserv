/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:12 by etran             #+#    #+#             */
/*   Updated: 2022/09/26 15:20:06 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "Server.hpp"

Server::Server(Block& x, Server::listOfServers servers) :
	_socket(socket(PF_INET, SOCK_STREAM, 0)),
	_epoll(&x, servers),
	_servers(servers) {
		_socket.setToReusable();
		memset(&_addr, 0, sizeof(_addr)); 
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(x.getPort());
		_addr.sin_addr.s_addr = inet_addr(x.getHost().c_str());

		if (bind(getSocket(), reinterpret_cast<struct sockaddr*>(&_addr), sizeof(_addr)) < 0)
			throw std::runtime_error("bind error");
		_socket.unlockSocket();
		_socket.listenSocket();

		_socket.setSockaddr(_addr);
		_socket.displaySocketInfo();
		DEBUG("Server Block constructor");
}

Server::~Server() {
	DEBUG("Server Block destructor");
}

/* PUBLIC =================================================================== */

void Server::launchServer() {
	_epoll.startMonitoring(_socket.getFd());
}

// Getter ----------------------------------------

int Server::getPort() const {
	return (ntohs(_addr.sin_port));
}

in_addr_t Server::getHost() const {
	return (_addr.sin_addr.s_addr);
}

int Server::getEpoll() const {
	return (_epoll.getFd());
}

int Server::getSocket() const {
	return (_socket.getFd());
}

const struct sockaddr_in& Server::getAddr() const {
	return (_addr);
}
