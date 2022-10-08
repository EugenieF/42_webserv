/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:12 by etran             #+#    #+#             */
/*   Updated: 2022/10/08 15:58:49 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "Server.hpp"

Server::Server(Block* x) :
	_socket(socket(PF_INET, SOCK_STREAM, 0)),
	_ip(x->getHost()) {
		_socket.setToReusable();
		memset(&_addr, 0, sizeof(_addr));
		_addr.sin_family = AF_INET;
		_addr.sin_port = htons(x->getPort());
		_addr.sin_addr.s_addr = inet_addr(_ip.c_str());

		if (bind(getSocket(), reinterpret_cast<struct sockaddr*>(&_addr),
				sizeof(_addr)) < 0)
			throw std::runtime_error("Server() bind error");
		_socket.unlockSocket();
		_socket.listenSocket();

		_displayServer();
		DEBUG("Server Block constructor");
	}

Server::~Server() {
	DEBUG("Server Block destructor");
}

/* PUBLIC =================================================================== */

// Getter ----------------------------------------

int Server::getPort() const {
	return (ntohs(_addr.sin_port));
}

const std::string& Server::getHost() const {
	return (_ip);
}

int Server::getSocket() const {
	return (_socket.getFd());
}

const struct sockaddr_in& Server::getAddr() const {
	return (_addr);
}

// Boolean operator ------------------------------

bool	Server::operator==(int fd) const {
	return (getSocket() == fd);
}

bool	Server::operator!=(int fd) const {
	return (getSocket() != fd);
}

bool	Server::operator==(const Server& rhs) const {
	return (getSocket() == rhs.getSocket());
}

bool	Server::operator!=(const Server& rhs) const {
	return (getSocket() != rhs.getSocket());
}

bool	Server::operator<(const Server& rhs) const {
	return (getSocket() < rhs.getSocket());
}

bool	Server::operator>(const Server& rhs) const {
	return (getSocket() > rhs.getSocket());
}

bool	Server::operator<=(const Server& rhs) const {
	return (getSocket() <= rhs.getSocket());
}

bool	Server::operator>=(const Server& rhs) const {
	return (getSocket() >= rhs.getSocket());
}

/* PRIVATE ================================================================== */

// Debug -----------------------------------------

void Server::_displayServer() const {
	std::cout	<< "== Connection infos ==" << NL
				<< "fd: " << getSocket() << NL
				<< "ip: " << getHost() << NL
				<< "port: " << getPort() << NL;
}
