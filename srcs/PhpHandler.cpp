/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhpHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eli <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 08:52:48 by eli               #+#    #+#             */
/*   Updated: 2022/09/26 13:42:04 by eli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "PhpHandler.hpp"

/* == PUBLIC =============================================================== */

PhpHandler::PhpHandler(const int sock) :
	ACgiHandler(sock, std::string(PHP_PATH)) {}

PhpHandler::~PhpHandler() {}

void PhpHandler::execute(const char* file, char* const* env) {
	char*	av[] = {
		const_cast<char*>(file),
		const_cast<char*>(getProgram()),
		0 };
	execve(av[0], av, env);
}
