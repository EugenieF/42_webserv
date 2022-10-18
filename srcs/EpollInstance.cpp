/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollInstance.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:37:04 by etran             #+#    #+#             */
/*   Updated: 2022/10/18 14:53:31 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "EpollInstance.hpp"

EpollInstance::EpollInstance() :
	_efd(-1) {
	memset(_events, 0, sizeof(struct epoll_event) * MAX_EVENT);
}

EpollInstance::~EpollInstance() {
	_clearClientList();
	_closeEfd();
}

/* == PUBLIC =============================================================== */

// Epoll manipulation ---------------------------

void EpollInstance::startMonitoring(serverMap& servers) {
	/* Creating fd associated with epoll */
	_efd = epoll_create1(0);
	_monitorServers(servers);

	while (getTriggeredValue() == false) {
		/* Waiting for events on every server sockets */
		int nb_sockets = epoll_wait(_efd, _events, MAX_EVENT, -1);
		if (nb_sockets < 0) {
			/* A signal ended the call */
			if (errno == EINTR)
				return ;
			else
				throw std::runtime_error("epoll_wait error");
		}
		for (int i = 0; i < nb_sockets; i++) {
			if (_events[i].events & EPOLLHUP || _events[i].events & EPOLLERR) {
				/* Checking if a client hung up OR an error occurred */
				if (close(_events[i].data.fd) < 0)
					throw std::runtime_error("epollmonitoring (close) error");
				_clientlist.erase(_events[i].data.fd);
				continue ;
			} else if (_events[i].events & EPOLLIN) {
				/* Incomming connection on a server OR request from client */
				serverMap::const_iterator	server_it;
				server_it = _findServerConnection(_events[i].data.fd, servers);
				if (server_it != servers.end())
					_processConnections(server_it);
				else
					_handleRequest(_clientlist[_events[i].data.fd]);
			} else if (_events[i].events & EPOLLOUT) {
				/* Client waiting a response */
				_handleResponse(_clientlist[_events[i].data.fd]);
			}
		}
	}
}

// Getter ---------------------------------------

int EpollInstance::getFd() const {
	return (_efd);
}

/* == PRIVATE ============================================================== */

// Epoll manipulation ---------------------------

void EpollInstance::_closeEfd() {
	if (_efd != -1)
		if (close(_efd) < 0)
			throw std::runtime_error("closing epoll_fd error");
}

void EpollInstance::_addSocket(int sock, int flag) {
	struct epoll_event	event;

	memset(&event, 0, sizeof(epoll_event));
	event.data.fd = sock;
	event.events = flag;
	if (epoll_ctl(_efd, EPOLL_CTL_ADD, sock, &event) < 0)
		throw std::runtime_error("adding socket to epoll led to error");
}

void EpollInstance::_editSocket(int sock, int flag) {
	struct epoll_event	event;

	memset(&event, 0, sizeof(epoll_event));
	event.data.fd = sock;
	event.events = flag;
	if (epoll_ctl(_efd, EPOLL_CTL_MOD, sock, &event) < 0)
		throw std::runtime_error("editing socket in epoll led to error");
}

void EpollInstance::_removeSocket(int sock) {
	if (epoll_ctl(_efd, EPOLL_CTL_DEL, sock, 0) < 0)
		throw std::runtime_error("removing socket in epoll led to error");
}

// Client list management -----------------------

/* Adding this new client to the epoll instance */
void EpollInstance::_addClient(serverMap::const_iterator it, int sock) {
	TcpSocket newsocket(sock, false);

       newsocket.setToReusable();
       newsocket.unlockSocket();
       _addSocket(sock, EPOLLIN);
       _clientlist[sock] = new Client(*it, sock, it->first->getEnv());
}

/* Removing the client from the server and from the client map */
void EpollInstance::_eraseClient(Client* client) {
       int clientfd;

       clientfd = client->getFd();
       _removeSocket(clientfd);
       if (close(clientfd) < 0)
               throw std::runtime_error("_eraseClient (close) error");
       _clientlist.erase(clientfd);
       delete client;
}

/* Clearing client map */
void EpollInstance::_clearClientList() {
	listOfClients::iterator ite = _clientlist.begin();

	while (ite != _clientlist.end()) {
		_eraseClient(ite->second);
		ite = _clientlist.begin();
	}
}

// Server connection management -----------------

/* Finding which server triggered the EPOLLIN call */
EpollInstance::serverMap::const_iterator
	EpollInstance::_findServerConnection(int fd, const serverMap& serv) const {
		for (serverMap::const_iterator it = serv.begin(); it != serv.end(); it++) {
			if (it->first->getSocket() == fd)
				return (it);
		}
		return (serv.end());
}

/* Adding server sockets to the epoll instance */
void   EpollInstance::_monitorServers(const serverMap& serverlist) {
	for (serverMap::const_iterator it = serverlist.begin();
			it != serverlist.end(); it++)
				_addSocket(it->first->getSocket(), EPOLLIN);
}

/* Checks whether the notification occurred unexpectedly,
   the event is then ignored and epoll keeps on checking
   the other events. */

void EpollInstance::_processConnections(serverMap::const_iterator it) {
	while (1) {
		struct sockaddr_in	cl_addr;
		socklen_t			cl_addr_len = sizeof(struct sockaddr_in);
		int					sock;
		
		memset(&cl_addr, 0, cl_addr_len);
		sock = accept(it->first->getSocket(),
					reinterpret_cast<struct sockaddr*>(&cl_addr), &cl_addr_len);
		if (sock < 0) {
		/* Every connection has been processed */
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				break ;
			else
				throw std::runtime_error("accept error");
		}
		displayMsg(" 🤝 New connection accepted", LIGHT_GREEN);
		std::cout << "server accessed : " << it->first->getSocket() << NL;
		std::cout << "client : " << sock << NL;
		_addClient(it, sock);
	}
}

void EpollInstance::_handleRequest(Client* client) {
	DEBUG("Request");
	std::string		str;

	try {
		str = readFd(client->getFd());
	} catch ( const std::exception& e) {
		std::cerr << e.what() << NL;
	}
	std::cout << RED << "******* str = " << str << RESET << std::endl;
	t_requestStatus requestStatus = client->parseRequest(str);
	if (requestStatus == COMPLETE_REQUEST) {
		_editSocket(client->getFd(), EPOLLOUT);
	}
}

void EpollInstance::_handleResponse(Client* client) {
	DEBUG("Response");
	//Response* response = client->generateResponse();
	client->generateResponse();
	std::string response = client->getResponse()->getResponse();
	if (write(client->getFd(), response.c_str(), response.size()) < 0)
		throw std::runtime_error("handleResponse (write) error");
	_eraseClient(client);
}
