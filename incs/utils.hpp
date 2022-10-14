/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:56:38 by etran             #+#    #+#             */
/*   Updated: 2022/10/14 21:10:19 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <cstdlib>
# include <sstream>
# include <signal.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>

# include "StatusCode.hpp"
# include "MimeType.hpp"
# include "HttpMethod.hpp"
# include "Colors.hpp"
# include "debug.hpp"

# define INIT_SIGNAL 0
# define RESET_SIGNAL 1

extern bool	_triggered;

void	DEBUG(const std::string& str);

		/*----------   Signal   ---------*/

void			setupSignal(int state);
bool			getTriggeredValue();

		/*-------- String related -------*/

std::string		convertSizeToString(size_t size);
std::string		convertNbToString(int nb);
std::string		convertCharPtrToString(char* ptr);
bool			convertPort(const std::string& str, int* port);
bool			convertHttpCode(const std::string& str, int* code);
std::string		trimSpacesStr(std::string *str, const char *toTrim = " \t");
std::string		trimSpacesEndStr(std::string *str, const char *toTrim = " \t");

		/*--------  File related  -------*/

bool			pathIsFile(const std::string& path);
bool			pathIsDirectory(const std::string& path);
bool			pathIsAccessible(const std::string& path);

		/*----------   Display  ---------*/

void			displayMsg(const std::string& msg, const char* colorCode);

#endif
