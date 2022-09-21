/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:07 by etran             #+#    #+#             */
/*   Updated: 2022/09/21 11:21:44 by efrancon         ###   ########.fr       */
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
# include <string.h>

# include "TcpSocket.hpp"
# include "Block.hpp"
# include "EpollInstance.hpp"

class Server {
	public:
		Server(Block& x);
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
};

#endif
