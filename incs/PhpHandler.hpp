/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhpHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eli <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 08:49:17 by eli               #+#    #+#             */
/*   Updated: 2022/09/30 12:39:42 by eli              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHPHANDLER_HPP
# define PHPHANDLER_HPP

# include "ACgiHandler.hpp"

# define PHP_PATH "/usr/bin/php7.4"

class PhpHandler : public virtual ACgiHandler {
	public:
		PhpHandler(const int sock);
		virtual ~PhpHandler();

		void		execute(const char* file, char* const* env);
};

#endif
