/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollInstance.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:36:56 by etran             #+#    #+#             */
/*   Updated: 2022/10/04 17:43:39 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EPOLLINSTANCE_HPP
# define EPOLLINSTANCE_HPP

# include <errno.h>
# include <exception>
# include <string.h>
# include <sys/epoll.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <unistd.h>
# include <string>
# include <set>

# include "TcpSocket.hpp"
# include "utils.hpp"
# include "Client.hpp"

# define BUFSIZE 2048
# define MAX_EVENT 100

class EpollInstance {
	public:
		/* -- Typedef ------------------------------------------------------ */
		typedef		Parser::listOfServers				listOfServers;
		typedef		Client*								ClientPtr;
		typedef		std::map<int, ClientPtr>::iterator	it;
		typedef		std::map<TcpSocket, Block*>			listOfSockets;

		EpollInstance();
		virtual ~EpollInstance();

		/* -- Epoll manipulation ------------------------------------------- */
		void						startMonitoring(char* const* env);

		/* -- Getter ------------------------------------------------------- */
		int							getFd() const;
		ClientPtr					getClient(int fd);

		void						setSocketList(listOfSockets sockets);

	private:
		/* -- Epoll manipulation ------------------------------------------- */
		void						_closeFd();

		/* -- Socket management -------------------------------------------- */
		void						_addSocket(int socket, int flag);
		void						_addSockets();
		void						_editSocket(int socket, int flag);
		void						_removeSocket(int socket);
		void						_clearClients();
		void						_clearClient(int fd, Client* client);

		Block*						_findServer(int fd);

		/* -- Connection management ---------------------------------------- */
		void						_processConnections(int serverSocket, Block* server);
		void						_handleRequest(int index);
		void						_handleResponse(int index, char* const* env);

		int							_efd;
		// int							_serversocket;
		struct epoll_event			_events[MAX_EVENT];
		std::map<int, ClientPtr>	_clientlist;
		listOfSockets				_socketList;
};

#endif
