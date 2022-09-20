/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:07 by etran             #+#    #+#             */
/*   Updated: 2022/09/20 15:16:28 by etran            ###   ########.fr       */
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
		Server(const Block& x);
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
