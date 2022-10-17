/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 16:56:38 by etran             #+#    #+#             */
/*   Updated: 2022/10/17 23:27:09 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <cstdlib>
# include <sstream>
# include <ctime>
# include <signal.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <stdlib.h>

# include "StatusCode.hpp"
# include "MimeType.hpp"
# include "HttpMethod.hpp"
# include "Colors.hpp"
# include "debug.hpp"

# define INIT_SIGNAL 0
# define RESET_SIGNAL 1
# define BUFSIZE 2048

extern bool	_triggered;

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
std::string		readFd(int fd);
char*			clone_str(const std::string& str);

		/*--------  File related  -------*/

bool			pathIsFile(const std::string& path);
bool			pathIsDirectory(const std::string& path);
bool			pathIsAccessible(const std::string& path);

		/*----------   Display  ---------*/

void			DEBUG(const std::string& str);
void			displayMsg(const std::string& msg, const char* colorCode);

		/*------------   Time   ---------*/

std::string		getFormattedDate();
std::string		getFormattedDate(std::time_t time);

#endif
