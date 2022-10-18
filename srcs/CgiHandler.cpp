/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 16:35:16 by etran             #+#    #+#             */
/*   Updated: 2022/10/18 19:51:09 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"
#include "Response.hpp"

/* == PUBLIC =============================================================== */

CgiHandler::CgiHandler(Response& response) :
	_program(response.getCgiProgram()),
	_socket(response.getFd()),
	_out(-1),
	_env(response.getEnv()),
	_script(response.getCgiName()) {}

CgiHandler::~CgiHandler() {
	_restore();
}

// Getter ---------------------------------------

/* Program that'll be executed */
const std::string& CgiHandler::getProgram() const {
	return (_program);
}

/* Name of the script */
const std::string& CgiHandler::getScript() const {
	return (_script);
}

/* Client fd */
int	CgiHandler::getFd() const {
	return (_socket);
}

/* Env with metavar */
char* const* CgiHandler::getEnv() const {
	return (_env);
}

// Cgi ------------------------------------------

std::string CgiHandler::getCgiOutput() {
	std::string	output;
	int			pid;

	DEBUG("In CGI!!");
	if (pipe(_fds) < 0)
		throw std::runtime_error("cgihandler getCgiOutput (pipe) error");
	pid = fork();
	if (pid < 0) {
		throw std::runtime_error("cgihandler getCgiOutput (fork) error");
	} else if (pid == 0) {
		_execute();
		exit(EXIT_FAILURE);
	}
	_waitForChild(pid);
	output = readFd(_fds[0]);
	close(_fds[0]);
	DEBUG("Leaving CGI!!");
	return (output);
}

/* == PRIVATE ============================================================== */

// Utils ----------------------------------------

void CgiHandler::_execute() {
	char* av[] = {
		const_cast<char*>(getProgram().c_str()),
		const_cast<char*>(getScript().c_str()),
		0 };
	_redirectToSock();
	_redirectToPipe();
	execve(av[0], av, getEnv());
	close(_fds[1]);
	throw std::runtime_error("execve failed in cgi");
}

void CgiHandler::_restore() {
	if (dup2(_out, STDOUT_FILENO) < 0)
		throw std::runtime_error("cgihandler _restore (dup2) error");
	if (close(_out))
		throw std::runtime_error("cgihandler _restore (close) error");
}

void CgiHandler::_redirectToSock() {
	if ((_out = dup(STDOUT_FILENO)) < 0)
		throw std::runtime_error("cgihandler _redirectToSock (dup) error");
	if (dup2(_socket, STDOUT_FILENO) < 0)
		throw std::runtime_error("cgihandler _redirectToSock (dup2) error");
}

void CgiHandler::_redirectToPipe() {
	if (close(_fds[0]) < 0) /* Don't need input side */
		throw std::runtime_error("cgihandler _redirectToPipe (close) error");
	if (dup2(_fds[1], STDOUT_FILENO) < 0) /* Output will also be on fds[0] */
		throw std::runtime_error("cgihandler _redirectToPipe (dup2) error");
}

void CgiHandler::_waitForChild(int pid) {
	/* Parent */
	//int	wstatus;
	int	times_waited = 0;
	if (close(_fds[1]) < 0) /* Don't need output side */
		throw std::runtime_error("cgihandler getCgiOutput (close) error");

	while (1) {
		if (waitpid(pid, 0 /* &wstatus */, WNOHANG) == pid)
			break ; /* OK */
		usleep(50);
		if (times_waited++ == 20000)
			break ; /* Error : timeout */
	}
	/* check if exit was ok */
}
