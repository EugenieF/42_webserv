/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollInstance.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:36:56 by etran             #+#    #+#             */
/*   Updated: 2022/10/04 11:37:53 by efrancon         ###   ########.fr       */
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
# include "Parser.hpp"

# define BUFSIZE 2048
# define MAX_EVENT 100

class EpollInstance {
	public:
		/* -- Typedef ------------------------------------------------------ */
		typedef		Parser::listOfServers				listOfServers;
		typedef		Client*								ClientPtr;
		typedef		std::map<int, ClientPtr>::iterator	it;

		EpollInstance(listOfServers _servers);
		virtual ~EpollInstance();

		/* -- Epoll manipulation ------------------------------------------- */
		void						startMonitoring(int servsocket, char* const* env);

		/* -- Getter ------------------------------------------------------- */
		int							getFd() const;
		ClientPtr					getClient(int fd);

	private:
		/* -- Epoll manipulation ------------------------------------------- */
		void						_closeFd();

		/* -- Socket management -------------------------------------------- */
		void						_addSocket(int socket, int flag);
		void						_editSocket(int socket, int flag);
		void						_removeSocket(int socket);
		void						_clearClients();
		void						_clearClient(int fd, Client* client);

		/* -- Connection management ---------------------------------------- */
		void						_processConnections();
		void						_handleRequest(int index);
		void						_handleResponse(int index, char* const* env);

		int							_efd;
		int							_serversocket;
		struct epoll_event			_events[MAX_EVENT];
		std::map<int, ClientPtr>	_clientlist;
		listOfServers				_servers;
};

#endif
