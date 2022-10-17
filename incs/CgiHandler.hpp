/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:06:06 by etran             #+#    #+#             */
/*   Updated: 2022/10/17 14:56:48 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <stdexcept>
# include <unistd.h>
# include <string>
# include <sys/types.h>
# include <sys/wait.h>

# include "Env.hpp"

class Response;

class CgiHandler {
	public:
		CgiHandler(Response& response);
		virtual ~CgiHandler();

		/* -- Getter -------------------------------------------------------- */
		const std::string&	getProgram() const;
		const std::string&	getScript() const;
		int					getFd() const;
		std::string			getCgiOutput();
		//const Env&			getEnv() const;
		char* const*			getEnv() const;

	private:
		/* -- Utils --------------------------------------------------------- */
		void				_execute();
		void				_restore();
		void				_redirectToSock();
		void				_redirectToPipe();
		void				_waitForChild(int pid);

		/* -- Data members -------------------------------------------------- */
		std::string			_program;
		int					_socket;
		int					_out;
		const Env&			_env;
		std::string			_script;
		int					_fds[2];
};

#endif
