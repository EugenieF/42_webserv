/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpSocket.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:34:52 by etran             #+#    #+#             */
/*   Updated: 2022/09/20 17:09:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "TcpSocket.hpp"

TcpSocket::TcpSocket() :
	_sockfd(-1),
	_autoclosing(true) {
		DEBUG("Tcp default constructor");
	}

TcpSocket::TcpSocket(int val, bool autoclose) :
	_sockfd(val),
	_autoclosing(autoclose) {
		if (val < 0)
			throw std::runtime_error("socket open error");
		DEBUG("Tcp val constructor");
	}

TcpSocket::~TcpSocket() {
	_closeFd();
	DEBUG("Tcp val destructor");
}

/* PUBLIC ================================================================== */

// Socket manipulation --------------------------

void TcpSocket::setToReusable() {
	int	_yes = 1;
	if (setsockopt(getFd(), SOL_SOCKET,
		SO_REUSEADDR | SO_REUSEPORT, &_yes, sizeof(_yes)) < 0)
		throw std::runtime_error("setsockopt error");
}

void TcpSocket::unlockSocket() {
	if (fcntl(getFd(), F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("fcntl error");
}

void TcpSocket::listenSocket() {
	if (listen(getFd(), LISTEN_VAL) < 0)
		throw std::runtime_error("listen error");
}

// Setter ---------------------------------------

void TcpSocket::setSockaddr(const struct sockaddr_in& addr) {
	_sockaddr = addr;
}

void TcpSocket::setAutoclosing(bool val) {
	_autoclosing = val;
}

// Getter ---------------------------------------

int TcpSocket::getFd() const {
	return (_sockfd);
}

const struct sockaddr_in& TcpSocket::getSockaddr() const {
	return (_sockaddr);
}

// DEBUG ----------------------------------------

void TcpSocket::displaySocketInfo() const {
	char				addr_buf[INET_ADDRSTRLEN + 1];

	memset(&addr_buf, 0, INET_ADDRSTRLEN + 1);
	// !! FORBIDDEN FUNCTION
	if (inet_ntop(AF_INET, &_sockaddr.sin_addr, addr_buf, INET_ADDRSTRLEN) == 0)
		throw std::runtime_error("inet ntop error");
	std::cout	<< "== Connection socket information ==" << NL
				<< " - fd: " << getFd() << NL
				<< " - host address: " << addr_buf << NL
				<< " - port: " << ntohs(_sockaddr.sin_port) << NL;
}

/* PRIVATE ================================================================= */

void TcpSocket::_closeFd() {
	if (getFd() != -1 && _autoclosing)
		if (close(_sockfd) < 0)
			throw std::runtime_error("close socket error");
}
