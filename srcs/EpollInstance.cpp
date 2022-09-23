/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollInstance.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:37:04 by etran             #+#    #+#             */
/*   Updated: 2022/09/23 16:27:36 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "debug.hpp"

# include "EpollInstance.hpp"

EpollInstance::EpollInstance(Block* x) : _efd(-1), _client(x) {
	DEBUG("Epoll constructor");
}

EpollInstance::~EpollInstance() {
	_clearClients();
	_closeFd();
	DEBUG("Epoll destructor");
}

/* == PUBLIC =============================================================== */

// Epoll manipulation ---------------------------

void EpollInstance::startMonitoring(int servsocket) {
	memset(&_events, 0, MAX_EVENT);
	_efd = epoll_create1(0);
	_serversocket = servsocket;
	_addSocket(_serversocket, EPOLLIN); // waiting for requests
	//_monitorSocket(_serversocket, EPOLL_CTL_ADD, EPOLLIN);

	while (getTriggeredValue() == false) {
		int nb_sockets = epoll_wait(_efd, _events, MAX_EVENT, -1);
		if (nb_sockets < 0) {
			if (errno == EINTR) {
				return ;
			} else {
				throw std::runtime_error("epoll_wait error");
			}
		}
		for (int i = 0; i < nb_sockets; i++) {
			if (_events[i].events & EPOLLHUP) {
				std::cout << "Socket " << _events[i].data.fd << " hung up" << NL;
				if (close(_events[i].data.fd) < 0)
					throw std::runtime_error("close in epollmonitoring error");
				_clientlist.erase(_events[i].data.fd);
			} else if (_events[i].events & EPOLLERR
				/*|| !(_events[i].events & EPOLLIN
				|| _events[i].events & EPOLLOUT)*/) {
				if (close(_events[i].data.fd) < 0)
					throw std::runtime_error("close in epollmonitoring error");
				continue ;
			} else if (_events[i].data.fd == _serversocket) {
				_processConnections();
			} else if (_events[i].events & EPOLLIN) {
				std::cout << RED << "_events[i].data.fd : " << _events[i].data.fd << RESET << std::endl;
				_handleRequest(i);
			} else if (_events[i].events & EPOLLOUT) {
				_handleResponse(i); 
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

void EpollInstance::_closeFd() {
	if (_efd != -1) {
		//if (epoll_ctl(_efd, EPOLL_CTL_DEL, _serversocket, 0) < 0 || close(_efd) < 0)
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
void EpollInstance::_processConnections() {
	while (1) { // Accept every connections regarding the expected events
		struct sockaddr_in	cl_addr;
		socklen_t			cl_addr_len = sizeof(struct sockaddr_in);
		int					_sock;
		
		memset(&cl_addr, 0, cl_addr_len);
		_sock = accept(_serversocket, reinterpret_cast<struct sockaddr*>(&cl_addr), &cl_addr_len);
		if (_sock < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) // Every connection has been processed
				break ;
			 else
				throw std::runtime_error("accept error");
		}
		DEBUG("New connection accepted !!");
		TcpSocket newsocket(_sock, false);
		newsocket.setToReusable();
		/* DEBUG */
		if (getsockname(newsocket.getFd(), reinterpret_cast<struct sockaddr*>(&cl_addr), &cl_addr_len) < 0)
			throw std::runtime_error("getsockname error");
		newsocket.setSockaddr(cl_addr);
		newsocket.displaySocketInfo();
		/* -===- */
		newsocket.unlockSocket();
		_addSocket(newsocket.getFd(), EPOLLIN); // newsocket waiting for a response
		_clientlist.insert(newsocket.getFd());
		//_monitorSocket(newsocket.getFd(), EPOLL_CTL_ADD, EPOLLIN);
	}
}

void EpollInstance::_handleRequest(int index) {
	DEBUG("Request");
	std::string		str;
	char			buf[BUFSIZE + 1];
	ssize_t			count;

	memset(buf, 0, BUFSIZE + 1);
	count = read(_events[index].data.fd, buf, BUFSIZE);
	while (count) {
		if (count < 0 /* && errno != EAGAIN */) {
			throw std::runtime_error("handleRequest (read) error");
		}
		str += buf;
		if (count == BUFSIZE) {
			memset(buf, 0, BUFSIZE + 1);
			count = read(_events[index].data.fd, buf, BUFSIZE);
		} else {
			break ;
		}
	}
	/******************************************************************************************/
	t_requestStatus requestStatus = _client.parseRequest(str);
	std::cout << YELLOW << "HANDLE REQUEST : fd " << _events[index].data.fd << ": " << requestStatus << RESET << NL;
	if (requestStatus == INVALID_REQUEST)
		_removeSocket(_events[index].data.fd);
	else if (requestStatus == COMPLETE_REQUEST)
		_editSocket(_events[index].data.fd, EPOLLOUT);
	/*******************************************************************************************/

	// std::cout << "Msg read from " << _events[index].data.fd << ": \n" << str << NL;
	// _editSocket(_events[index].data.fd, EPOLLOUT);
}

void EpollInstance::_handleResponse(int index) {
	DEBUG("Response");
	// std::string	str =
	// 	"<!DOCTYPE html>\n\
	//   	<html><head>\n\
	//   	<title>First try</title>\n\
	//   	</head>\n\
	//   	<body><p>Hello world!</p></body>\n\
	//   	</html>";

	/******************************************************************************************/
	std::string responseMsg = _client.generateResponse();
	
	if (write(_events[index].data.fd, responseMsg.c_str(), responseMsg.size()) < 0)
		throw std::runtime_error("handleResponse (write) error");
	_client.clear();
	/******************************************************************************************/

	// if (write(_events[index].data.fd, str.c_str(), str.size()) < 0)
		// throw std::runtime_error("handleResponse (write) error");
	_removeSocket(_events[index].data.fd);
	_editSocket(_events[index].data.fd, EPOLLIN);
	//if (close(_events[index].data.fd) < 0)
	//	throw std::runtime_error("handleResponse (close) error");
	
	// cgi or page display

}

void EpollInstance::_clearClients() {
	for (it client = _clientlist.begin(); client != _clientlist.end(); client++) {
		if (close(*client) < 0)
			throw std::runtime_error("clearlist (close) error");
	}
}
