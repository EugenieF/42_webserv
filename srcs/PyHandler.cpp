/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PyHandler.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eli <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 08:52:48 by eli               #+#    #+#             */
/*   Updated: 2022/09/30 12:42:43 by eli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "PyHandler.hpp"

PyHandler::PyHandler(const int sock) :
	ACgiHandler(sock, std::string(PYTHON_PATH)) {}

PyHandler::~PyHandler() {}

void PyHandler::execute(const char* file, char* const* env) {
	char*	av[] = {
		const_cast<char*>(file),
		const_cast<char*>(getProgram()),
		0 };
	execve(av[0], av, env);
}
