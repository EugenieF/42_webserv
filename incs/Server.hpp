/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:07 by etran             #+#    #+#             */
/*   Updated: 2022/10/06 11:41:49 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

/* Class will control
   - sockadd_in structure
   - epoll instance (and everything monitoring related)
   - socket connections
*/

# include <exception>
# include <sys/types.h>
# include <sys/socket.h>

# include "Block.hpp"
# include "EpollInstance.hpp"
# include "Parser.hpp"
# include "utils.hpp"

class Server {
	public:
		typedef Parser::listOfServers			listOfServers;
		typedef EpollInstance::listOfSockets	listOfSockets;

		Server(listOfServers servers, char* const* env);
		virtual ~Server();

		/* -- Server management -------------------------------------------- */
		void							launchServer();

		/* -- Getter ------------------------------------------------------- */
		// int								getPort() const;
		// const std::string&				getHost() const;
		int								getEpoll() const;
		// int								getSocket() const;
		// const struct sockaddr_in&		getAddr() const;

	private:
		/* -- Debug -------------------------------------------------------- */
		void							_displayServer(Block *server) const;

		void							_createSocketList();	
		TcpSocket						_createSocket(int port, const std::string& ipAddress);

		// TcpSocket						_socket;
		listOfSockets					_socketList;
		EpollInstance					_epoll;
		// struct sockaddr_in				_addr;
		std::string						_ip;
		char* const*					_env;
		listOfServers					_servers;
};

#endif
