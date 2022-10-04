/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PyHandler.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eli <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 08:49:17 by eli               #+#    #+#             */
/*   Updated: 2022/09/30 12:39:48 by eli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PYHANDLER_HPP
# define PYHANDLER_HPP

# include "ACgiHandler.hpp"

# define PYTHON_PATH ""

class PyHandler : public virtual ACgiHandler {
	public:
		PyHandler(const int sock);
		virtual ~PyHandler();

		void		execute(const char* file, char* const* env);
};

#endif
