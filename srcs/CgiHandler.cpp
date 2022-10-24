/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 16:35:16 by etran             #+#    #+#             */
/*   Updated: 2022/10/21 15:48:59 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"
#include "Response.hpp"

/* == PUBLIC =============================================================== */

CgiHandler::CgiHandler(Response& response) :
	_program(response.getCgiProgram()),
	_in(-1),
	_out(-1),
	_env(response.getEnv()),
	_script(response.getBuiltPath()),
	_requestbody(response.getRequest()->getBody()) {}

CgiHandler::~CgiHandler() {
	_restore();
	DEBUG("Leaving CGI!!");
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

/* Env with metavar */
char* const* CgiHandler::getEnv() const {
	return (_env);
}

// Cgi ------------------------------------------

std::string CgiHandler::getCgiOutput() {
	std::string	output;
	int			pid;

	DEBUG("In CGI!!");
	if (pipe(_fd_in) < 0)
		throw std::runtime_error("cgihandler getCgiOutput (pipe) error");
	if (pipe(_fd_out) < 0)
		throw std::runtime_error("cgihandler getCgiOutput (pipe) error");
	_setPipeNoBlock();
	//_writeToStdin();
	pid = fork();
	if (pid < 0) {
		throw std::runtime_error("cgihandler getCgiOutput (fork) error");
	} else if (pid == 0) {
		/* Child */
		try {
			_execute();
		} catch (const std::exception &e) {
			std::cerr << "Execute fail: " << e.what() << NL; /* DEBUG */
			exit(EXIT_FAILURE);
		}
	}
	/* Parent */
	_waitForChild(pid);
	output = readFd(_fd_out[0]);
	close(_fd_out[0]);
	std::cerr << RED << "OUTPUT :\n" << output << RESET << NL;
	return (output);
}

/* == PRIVATE ============================================================== */

// Utils ----------------------------------------

void CgiHandler::_execute() {
	char* av[] = {
		const_cast<char*>(getProgram().c_str()),
		const_cast<char*>(getScript().c_str()),
		0 };
	_redirectToPipe();
	execve(av[0], av, getEnv());
	if (close(_fd_in[0]))
		throw std::runtime_error("close in fork failed in cgi");
	if (close(_fd_out[1]))
		throw std::runtime_error("close in fork failed in cgi");
	throw std::runtime_error("execve failed in cgi");
}

void CgiHandler::_restore() {
	if (_in == -1)
		return ;
	if (dup2(_in, STDIN_FILENO) < 0)
		throw std::runtime_error("cgihandler _restore (dup2) error");
	if (close(_in))
		throw std::runtime_error("cgihandler _restore (close) error");

	if (_out == -1)
		return ;
	if (dup2(_out, STDOUT_FILENO) < 0)
		throw std::runtime_error("cgihandler _restore (dup2) error");
	if (close(_out))
		throw std::runtime_error("cgihandler _restore (close) error");
}

/* Redirect output to the pipe -> building response */
void CgiHandler::_redirectToPipe() {
	if (close(_fd_in[1]) < 0)
		throw std::runtime_error("cgihandler _redirectToPipe (close) error");
	if ((_in = dup(STDIN_FILENO)) < 0) /* Save old stdout */
		throw std::runtime_error("cgihandler _redirectToSock (dup) error");
	if (dup2(_fd_in[0], STDIN_FILENO) < 0) /* Output will also be on fd_out[0] */
		throw std::runtime_error("cgihandler _redirectToPipe (dup2) error");

	if (close(_fd_out[0]) < 0) /* Don't need input side */
		throw std::runtime_error("cgihandler _redirectToPipe (close) error");
	if ((_out = dup(STDOUT_FILENO)) < 0) /* Save old stdout */
		throw std::runtime_error("cgihandler _redirectToSock (dup) error");
	if (dup2(_fd_out[1], STDOUT_FILENO) < 0) /* Output will also be on fd_out[0] */
		throw std::runtime_error("cgihandler _redirectToPipe (dup2) error");
}

void CgiHandler::_waitForChild(int pid) {
	/* Parent */
	int	wstatus;
	int	times_waited = -1;

	if (close(_fd_in[0]) < 0)
		throw INTERNAL_SERVER_ERROR;
	if (close(_fd_out[1]) < 0) /* Don't need output side */
		throw INTERNAL_SERVER_ERROR;
		//throw std::runtime_error("cgihandler _waitForChild (close) error");
	_writeToStdin();

	while (1) {
		if (waitpid(pid, &wstatus, WNOHANG) == pid)
			break ; /* OK */
		usleep(50);
		if (++times_waited == 20000)
			break ; /* OK */
			//throw std::runtime_error("cgihandler waitForChild (waitpid) error");
			//break ; /* Error : timeout */
	}
	/* check if exit was ok */
	if (WIFEXITED(wstatus)) {
		if (WEXITSTATUS(wstatus) != EXIT_FAILURE) {
			DEBUG ("Exited waitforchild smoothh..... ==== ");
			return ; /* exited */
		}
	}
	close(_fd_out[0]); // ERROR
	throw INTERNAL_SERVER_ERROR;
}

void CgiHandler::_setPipeNoBlock() {
	//if (fcntl(_fd_in[0], F_SETFL, O_NONBLOCK))
	//	throw std::runtime_error("cgihandler _setPipeNoBlock (fcntl) error");
	//if (fcntl(_fd_in[1], F_SETFL, O_NONBLOCK))
	//	throw std::runtime_error("cgihandler _setPipeNoBlock (fcntl) error");
	if (fcntl(_fd_out[0], F_SETFL, O_NONBLOCK))
		throw std::runtime_error("cgihandler _setPipeNoBlock (fcntl) error");
	if (fcntl(_fd_out[1], F_SETFL, O_NONBLOCK))
		throw std::runtime_error("cgihandler _setPipeNoBlock (fcntl) error");
}

void CgiHandler::_writeToStdin() {
	if (write(_fd_in[1], _requestbody.c_str(), _requestbody.size()) < 0)
		throw std::runtime_error("cgihandler _writeToStdin (write) error");
	if (close(_fd_in[1]) < 0)
		throw std::runtime_error("cgihandler _writeToStdin (close) error");
}
