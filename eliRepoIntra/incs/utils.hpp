/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etran <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:56:38 by etran             #+#    #+#             */
/*   Updated: 2022/09/19 17:10:44 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <signal.h>

# define INIT_SIGNAL 0
# define RESET_SIGNAL 1

extern bool	_triggered;

void	setupSignal(int state);
bool	getTriggeredValue();

#endif
