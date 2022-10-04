/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACgiHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/22 14:06:06 by etran             #+#    #+#             */
/*   Updated: 2022/09/30 12:42:02 by eli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACGIHANDLER_HPP
# define ACGIHANDLER_HPP

# include <stdexcept>
# include <unistd.h>
# include <string>

class ACgiHandler {
	public:
		explicit ACgiHandler(const int sock, const std::string& program);
		virtual ~ACgiHandler();

		const char*		getProgram() const;
		virtual void	execute(const char* file, char* const* env) = 0;

	private:
		void			_restore();

		const char*		_program;
		int				_in;
		int				_out;
};

#endif
