/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:28:07 by etran             #+#    #+#             */
/*   Updated: 2022/10/08 15:58:38 by efrancon         ###   ########.fr       */
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

# include "TcpSocket.hpp"
# include "Parser.hpp"

class Server {
	public:
		typedef Parser::listOfServers	listOfServers;

		Server(Block* x);
		virtual ~Server();

		/* -- Getter ------------------------------------------------------- */
		int								getPort() const;
		const std::string&				getHost() const;
		int								getSocket() const;
		const struct sockaddr_in&		getAddr() const;

		/* -- Boolean operator --------------------------------------------- */
		bool							operator==(int fd) const;
		bool							operator!=(int fd) const;
		bool							operator==(const Server& rhs) const;
		bool							operator!=(const Server& rhs) const;
		bool							operator<(const Server& rhs) const;
		bool							operator>(const Server& rhs) const;
		bool							operator<=(const Server& rhs) const;
		bool							operator>=(const Server& rhs) const;

	private:
		/* -- Debug -------------------------------------------------------- */
		void							_displayServer() const;

		TcpSocket						_socket;
		struct sockaddr_in				_addr;
		std::string						_ip;
};

#endif
