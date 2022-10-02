/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:56:38 by etran             #+#    #+#             */
/*   Updated: 2022/10/01 16:51:07 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <sstream>
# include <signal.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>

# include "StatusCode.hpp"
# include "MimeType.hpp"
# include "HttpMethod.hpp"

# define INIT_SIGNAL 0
# define RESET_SIGNAL 1

extern bool	_triggered;

void			setupSignal(int state);
bool			getTriggeredValue();

		/*-------- String related -------*/

std::string		convertSizeToString(size_t size);
std::string		convertNbToString(int nb);
std::string		convertCharPtrToString(char* ptr);

		/*--------  File related  -------*/
bool			pathIsFile(const std::string& path);
bool			pathIsDirectory(const std::string& path);
bool			pathIsAccessible(const std::string& path);

#endif
