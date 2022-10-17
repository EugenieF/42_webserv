/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efrancon <efrancon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/19 15:30:51 by etran             #+#    #+#             */
/*   Updated: 2022/10/13 13:07:40 by etran            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
# include "debug.hpp"

int main(int argc, char **argv, char* const* env)
{
	std::string configFile("tester/testFiles/valid/default.conf");

	if (argc != 2)
	{
		DEBUG("no File");
		std::cerr << "usage : ./webserv [a configuration file]" << std::endl;
		if (argc > 2)
			return (EXIT_FAILURE);
		std::cout << "Using default configuration file" << std::endl;
	}
	else {
		DEBUG("File");
		configFile = argv[1];
	}
		//std::string configFile(argv[1]);
	try
	{
		//setupSignal(INIT_SIGNAL);
		Webserv	webserv(configFile, env);
		// webserv.displayServers();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cerr << "Errno value: " << errno << NL;
		//setupSignal(RESET_SIGNAL);
		return (EXIT_FAILURE);
	}
	//setupSignal(RESET_SIGNAL);
	return (EXIT_SUCCESS);
}
