/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EpollInstance.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 14:36:56 by etran             #+#    #+#             */
/*   Updated: 2022/10/14 21:21:57 by efrancon         ###   ########.fr       */
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

# include "Server.hpp"
# include "Client.hpp"

# define BUFSIZE 2048
# define MAX_EVENT 100

class EpollInstance {
	public:
		/* -- Typedef ------------------------------------------------------ */
		typedef		std::map<Server*, Block*>			serverMap;
		typedef		std::map<int, Client*>				listOfClients;
		//typedef		std::map<int, Client>::iterator		listOfClientsIte;
		//typedef		std::map<Server, Block*>::iterator	serverMapIte;


		EpollInstance();
		virtual ~EpollInstance();

		/* -- Epoll manipulation ------------------------------------------- */
		void						startMonitoring(serverMap& servers, char* const* env);

		/* -- Getter ------------------------------------------------------- */
		int							getFd() const;

	private:
		/* -- Epoll manipulation ------------------------------------------- */
		void						_closeFd();

		/* -- Socket management -------------------------------------------- */
		void						_addSocket(int socket, int flag);
		void						_editSocket(int socket, int flag);
		void						_removeSocket(int socket);

		/* -- Client list management --------------------------------------- */
		serverMap::const_iterator	_findServerConnection(int fd, const serverMap& serv) const;
		void						_eraseClient(Client* client);
		void						_clearClients();

		/* -- Server connection management --------------------------------- */
		void						_monitorServers(const serverMap& serverlist);
		void						_processConnections(serverMap::const_iterator it);
		void						_handleRequest(Client& client);
		void						_handleResponse(Client& client, char* const* env);

		int							_efd;
		struct epoll_event			_events[MAX_EVENT];
		listOfClients				_clientlist;

};

#endif
