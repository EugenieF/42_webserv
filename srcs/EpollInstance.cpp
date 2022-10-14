/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollInstance.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:37:04 by etran             #+#    #+#             */
/*   Updated: 2022/10/14 19:12:03 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "EpollInstance.hpp"

EpollInstance::EpollInstance() :
	_efd(-1) {
		DEBUG("Epoll constructor");
	}

EpollInstance::~EpollInstance() {
	_clearClients();
	_closeFd();
	DEBUG("Epoll destructor");
}

/* == PUBLIC =============================================================== */

void EpollInstance::startMonitoring(serverMap& servers, char* const* env) {
	memset(_events, 0, sizeof(struct epoll_event) * MAX_EVENT);
	_efd = epoll_create1(0);
	_monitorServers(servers);

	while (getTriggeredValue() == false) {
		int nb_sockets = epoll_wait(_efd, _events, MAX_EVENT, -1);
		if (nb_sockets < 0) {
			if (errno == EINTR)
				return ;
			else
				throw std::runtime_error("epoll_wait error");
		}
		for (int i = 0; i < nb_sockets; i++) {
			/* Checking whether a socket hung up OR an error occurred */
			if (_events[i].events & EPOLLHUP || _events[i].events & EPOLLERR) {
				if (close(_events[i].data.fd) < 0)
					throw std::runtime_error("epollmonitoring (close) error");
				_clientlist.erase(_events[i].data.fd);
				continue ;
			} else if (_events[i].events & EPOLLIN) {
				serverMap::const_iterator	ret;
				ret = _findServerConnection(_events[i].data.fd, servers);
				if (ret != servers.end())
					_processConnections(ret);
				else
					_handleRequest(*_clientlist[_events[i].data.fd]);
			} else if (_events[i].events & EPOLLOUT) {
				_handleResponse(*_clientlist[_events[i].data.fd], env); 
			}
		}
	}
}

EpollInstance::serverMap::const_iterator
	EpollInstance::_findServerConnection(int fd, const serverMap& serv) const {
		for (serverMap::const_iterator it = serv.begin(); it != serv.end(); it++) {
			if (it->first->getSocket() == fd)
				return (it);
		}
		return (serv.end());
	}

void	EpollInstance::_monitorServers(const serverMap& serverlist) {
	for (serverMap::const_iterator it = serverlist.begin(); it != serverlist.end(); it++) {
		_addSocket(it->first->getSocket(), EPOLLIN);
	}
}


// Getter ---------------------------------------

int EpollInstance::getFd() const {
	return (_efd);
}

/* == PRIVATE ============================================================== */

// Epoll manipulation ---------------------------

void EpollInstance::_closeFd() {
	if (_efd != -1) {
		if (close(_efd) < 0)
			throw std::runtime_error("closing epoll_fd error");
	}
}

// Socket management ----------------------------

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
	// if (epoll_ctl(_efd, EPOLL_CTL_MOD, sock, &event) < 0)
	if (epoll_ctl(_efd, EPOLL_CTL_MOD, sock, &event) < 0)
		throw std::runtime_error("editing socket in epoll led to error");
}

void EpollInstance::_removeSocket(int sock) {
	if (epoll_ctl(_efd, EPOLL_CTL_DEL, sock, 0) < 0)
		throw std::runtime_error("removing socket in epoll led to error");
}

// Connection management ------------------------

/* Checks whether the notification occurred unexpectedly,
   the event is then ignored and epoll keeps on checking
   the other events. */

//void EpollInstance::_processConnections(int serverSocket, Block* server) {
void EpollInstance::_processConnections(serverMap::const_iterator it) {
	while (1) {
		struct sockaddr_in	cl_addr;
		socklen_t			cl_addr_len = sizeof(struct sockaddr_in);
		int					_sock;
		
		memset(&cl_addr, 0, cl_addr_len);
		_sock = accept(it->first->getSocket(), reinterpret_cast<struct sockaddr*>(&cl_addr), &cl_addr_len);
		if (_sock < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) // Every connection has been processed
				break ;
			else
				throw std::runtime_error("accept error");
		}
		// DEBUG("New connection accepted !!");
		displayMsg(" 🤝 New connection accepted", LIGHT_GREEN);
		TcpSocket newsocket(_sock, false);

		newsocket.setToReusable();
		newsocket.unlockSocket();
		_addSocket(newsocket.getFd(), EPOLLIN); // newsocket waiting for a response
		_clientlist[newsocket.getFd()] = new Client(*it, newsocket.getFd());

		//_clientlist.insert(std::make_pair(newsocket.getFd(), new Client(server)));
		// std::cout << RED << "New Client --> fd: " << newsocket.getFd() << RESET << std::endl;
	}
}

//EpollInstance::ClientPtr	EpollInstance::getClient(int fd)
//{
//	ClientPtr	client;
//
//	if (_clientlist.find(fd) == _clientlist.end())
//		throw std::runtime_error("getClient (find) error");
//	client = _clientlist[fd];
//	return (client);
//}

void EpollInstance::_handleRequest(Client& client) {
	DEBUG("Request");
	std::string		str;
	char			buf[BUFSIZE + 1];

	ssize_t			count;
	t_requestStatus requestStatus;
	//Client&			client = _clientlist[clientfd];

	memset(buf, 0, BUFSIZE + 1);
	count = read(client.getFd(), buf, BUFSIZE);
	//client = getClient(_events[index].data.fd);
	while (count)
	{
		if (count < 0 /* && errno != EAGAIN */) {
			_eraseClient(&client);
			throw std::runtime_error("handleRequest (read) error");
		}
		str += buf;
		if (count == BUFSIZE) {
			memset(buf, 0, BUFSIZE + 1);
			count = read(client.getFd(), buf, BUFSIZE);
		} else {
			break ;
		}
	}
	requestStatus = client.parseRequest(str);
	if (requestStatus == COMPLETE_REQUEST) {
		_editSocket(client.getFd(), EPOLLOUT);
	}
}

//void EpollInstance::_handleResponse(int index, char* const* env) {
//	DEBUG("Response");
//
//	// REGULAR
//	//if (write(_events[index].data.fd, str.c_str(), str.size()) < 0)
//	//	throw std::runtime_error("handleResponse (write) error");
//	//_removeSocket(_events[index].data.fd);
//	//(void)env;
//
//	//SPECIAL
//	int pid = fork();
//	if (pid < 0) {
//		throw std::runtime_error("handleResponse (fork) error");
//	} else if (pid != 0) {
//		_removeSocket(_events[index].data.fd);
//	} else {
//		std::string	pgm("/usr/bin/php7.4");
//		std::string	file("tmp.php");
//		char*av[] = {
//			const_cast<char*>(pgm.c_str()),
//			const_cast<char*>(file.c_str()),
//			0 };
//		execve(av[0], av, env);
//	}
//	//cgi or page display
//}


//void EpollInstance::_handleResponse(int index, char* const* env) {
void EpollInstance::_handleResponse(Client& client, char* const* env) {
	DEBUG("Response");
	/* CHECK IF RESPONSE IS PAGE/CGI */

	// Case: PAGE
	(void)env;
	std::string responseMsg = client.generateResponse();

	if (write(client.getFd(), responseMsg.c_str(), responseMsg.size()) < 0)
		throw std::runtime_error("handleResponse (write) error");
	/*
	// Case: CGI THINGY
	// ... do stuff ...
	int pid = fork();
	if (pid < 0) {
		throw std::runtime_error("handleResponse (fork) error");
	} else if (pid != 0) {
		_removeSocket(_events[index].data.fd);
	} else {
		std::string	pgm("/usr/bin/php7.4");
		std::string	file("tmp.php");
		char*av[] = {
			const_cast<char*>(pgm.c_str()),
			const_cast<char*>(file.c_str()),
			0 };
		execve(av[0], av, env);
	}
	*/

	//_editSocket(_events[index].data.fd, EPOLLIN);	/* PROBLEM HERE */
	//_clearClient(_events[index].data.fd, client);
	_eraseClient(&client);
}
 
void EpollInstance::_eraseClient(Client* client) {
	int clientfd;

	clientfd = client->getFd();
	_removeSocket(clientfd);
	if (close(clientfd) < 0)
		throw std::runtime_error("_eraseClient (close) error");
	delete client;
	_clientlist.erase(clientfd);
}

void EpollInstance::_clearClients()
{
	for (listOfClients::const_iterator client = _clientlist.begin(); client != _clientlist.end(); client++) {
		_eraseClient(client->second);
	}
}
