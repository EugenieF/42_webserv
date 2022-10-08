/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 15:30:51 by etran             #+#    #+#             */
/*   Updated: 2022/10/04 16:53:37 by efrancon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int main(int argc, char **argv, char* const* env)
{
	std::string configFile("tester/testFiles/valid/default.conf");

	if (argc != 2)
	{
		std::cerr << "usage : ./webserv [a configuration file]" << std::endl;
		if (argc > 2)
			return (EXIT_FAILURE);
		std::cout << "Using default configuration file" << std::endl;
	}
	else
		std::string configFile(argv[1]);
	try
	{
		Webserv	webserv(configFile);
		setupSignal(INIT_SIGNAL);
		Server	serv(webserv.getServers(), env);
		serv.launchServer();
		// webserv.displayServers();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cerr << "Errno value: " << errno << NL;
		setupSignal(RESET_SIGNAL);
		return (EXIT_FAILURE);
	}
	setupSignal(RESET_SIGNAL);
	return (EXIT_SUCCESS);
}
