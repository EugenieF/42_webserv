/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:07 by etran             #+#    #+#             */
/*   Updated: 2022/10/04 11:00:06 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "TcpSocket.hpp"
# include "EpollInstance.hpp"

/******************************************************************************/
/*                                CLASS SERVER                                */
/******************************************************************************/

class Server
{
	private:
	/**********************     MEMBER VARIABLES     ********************/
		int			_fd;


class Server {
	public:
	/**********************************************************************************/
		typedef Parser::listOfServers	listOfServers;
	/**********************************************************************************/

		Server(Block& x, listOfServers servers);
		virtual ~Server();

		/* -- Server management -------------------------------------------- */
		void							launchServer();

		/* -- Getter ------------------------------------------------------- */
		int								getPort() const;
		in_addr_t						getHost() const;
		int								getEpoll() const;
		int								getSocket() const;
		const struct sockaddr_in&		getAddr() const;

	private:
		TcpSocket						_socket;
		EpollInstance					_epoll;
		struct sockaddr_in				_addr;
	/**********************************************************************************/
		listOfServers					_servers;
	/**********************************************************************************/

};

#endif