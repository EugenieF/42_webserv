/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACgiHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 16:35:16 by etran             #+#    #+#             */
/*   Updated: 2022/09/30 12:40:26 by eli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACgiHandler.hpp"

/* == PUBLIC =============================================================== */

ACgiHandler::ACgiHandler(const int sock, const std::string& program) :
	_program(program.c_str()),
	_in(dup(STDIN_FILENO)),
	_out(dup(STDOUT_FILENO)) {
		dup2(sock, STDOUT_FILENO);
	}

ACgiHandler::~ACgiHandler() {
	_restore();
}

// Getter ---------------------------------------
const char* ACgiHandler::getProgram() const {
	return (_program);
}

/* == PRIVATE ============================================================== */

void ACgiHandler::_restore() {
	if (dup2(_in, STDIN_FILENO) < 0 || dup2(_out, STDOUT_FILENO) < 0)
		throw std::runtime_error("cgihandler restore (dup2) error");
	if (close(_in) < 0 || close(_out))
		throw std::runtime_error("cgihandler restore (close) error");
}
