/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:56:38 by etran             #+#    #+#             */
/*   Updated: 2022/09/23 12:58:53 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <signal.h>
# include <sstream>

# define INIT_SIGNAL 0
# define RESET_SIGNAL 1

extern bool	_triggered;

void			setupSignal(int state);
bool			getTriggeredValue();

std::string		convertNbToString(size_t size);
std::string		convertNbToString(int nb);

#endif
