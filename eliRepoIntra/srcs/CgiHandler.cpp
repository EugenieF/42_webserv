/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 16:35:16 by etran             #+#    #+#             */
/*   Updated: 2022/09/24 17:34:50 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bla.hpp"

bla::bla(const int sock) :
	_sock(sock),
	_in(dup(STDIN_FILENO)),
	_out(dup(STDOUT_FILENO)) {
		dup2(sock, STDOUT_FILENO);
	} //dup

bla::~bla() {
	_restore();
} //reset dup

bla::_restore() {
	if (dup2(_in, STDIN_FILENO) < 0 || dup2(_out, STDOUT_FILENO) < 0)
		throw std::runtime_error("cgihandler restore (dup2) error");
	if (close(_in) < 0 || close(_out))
		throw std::runtime_error("cgihandler restore (close) error");
}
