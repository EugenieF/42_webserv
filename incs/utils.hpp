/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:56:38 by etran             #+#    #+#             */
/*   Updated: 2022/09/23 14:34:52 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <signal.h>
# include <sstream>

# include "StatusCode.hpp"
# include "MimeType.hpp"

# define INIT_SIGNAL 0
# define RESET_SIGNAL 1

extern bool	_triggered;

void			setupSignal(int state);
bool			getTriggeredValue();

std::string		convertSizeToString(size_t size);
std::string		convertNbToString(int nb);

#endif
