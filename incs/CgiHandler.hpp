/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:06:06 by etran             #+#    #+#             */
/*   Updated: 2022/10/20 17:43:00 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <stdexcept>
# include <unistd.h>
# include <string>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

# include "Env.hpp"

class Response;

class CgiHandler {
	public:
		CgiHandler(Response& response);
		virtual ~CgiHandler();

		/* -- Getter -------------------------------------------------------- */
		const std::string&	getProgram() const;
		const std::string&	getScript() const;
		char* const*		getEnv() const;

		std::string			getCgiOutput();

	private:
		/* -- Utils --------------------------------------------------------- */
		void				_execute();
		void				_restore();
		void				_redirectToPipe();
		void				_waitForChild(int pid);
		void				_setPipeNoBlock();
		void 				_writeToStdin();

		/* -- Data members -------------------------------------------------- */
		std::string			_program;
		int					_in;
		int					_out;
		const Env&			_env;
		std::string			_script;
		int					_fd_in[2];
		int					_fd_out[2];
		std::string			_requestbody;
};

#endif
