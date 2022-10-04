/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpSocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:35:13 by etran             #+#    #+#             */
/*   Updated: 2022/09/22 13:55:04 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSOCKET_HPP
# define TCPSOCKET_HPP

# include <string.h>
# include <exception>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <iostream>

# define LISTEN_VAL 10

class TcpSocket {
	public:
		TcpSocket();
		explicit TcpSocket(int val, bool autoclose = true);
		virtual ~TcpSocket();

		/* -- Socket manipulation ------------------------------------------ */
		void						setToReusable();
		void						unlockSocket();
		void						listenSocket();

		/* -- Getter ------------------------------------------------------- */
		int							getFd() const;

	private:
		/* -- Socket manipulation ------------------------------------------ */
		void						_closeFd();

		int							_sockfd;
		bool						_autoclosing;
};
#endif
