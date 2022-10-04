/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:12 by etran             #+#    #+#             */
/*   Updated: 2022/09/24 16:38:18 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "Server.hpp"

Server::Server(const Block& x, char* const* env) :
	_socket(socket(PF_INET, SOCK_STREAM, 0)),
	_ip(x.getHost()),
	_env(env) {
		_socket.setToReusable();
		memset(&_addr, 0, sizeof(_addr)); 
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(x.getPort());
		_addr.sin_addr.s_addr = inet_addr(_ip.c_str());

		if (bind(getSocket(), reinterpret_cast<struct sockaddr*>(&_addr), sizeof(_addr)) < 0)
			throw std::runtime_error("bind error");
		_socket.unlockSocket();
		_socket.listenSocket();

		_displayServer();
		DEBUG("Server Block constructor");
	}

Server::~Server() {
	DEBUG("Server Block destructor");
}

/* PUBLIC =================================================================== */

// Server management -----------------------------

void Server::launchServer() {
	_epoll.startMonitoring(_socket.getFd(), _env);
}

// Getter ----------------------------------------

int Server::getPort() const {
	return (ntohs(_addr.sin_port));
}

const std::string& Server::getHost() const {
	return (_ip);
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

/* PRIVATE ================================================================== */

// Debug -----------------------------------------

void Server::_displayServer() const {
	std::cout	<< "== Connection infos ==" << NL
				<< "fd: " << getSocket() << NL
				<< "ip: " << getHost() << NL
				<< "port: " << getPort() << NL;
}
