/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvinour <rvinour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:20:13 by rvinour           #+#    #+#             */
/*   Updated: 2023/12/15 14:19:27 by rvinour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include "Server.hpp"
#include <signal.h>

bool stop;

int parse(int ac, char **av)
{
	std::string ports = av[1];
	if (ac != 3)
		return (-1);
	if (atoi(av[1]) < 0 || atoi(av[1]) > 65536)
		return (-1);
	for (size_t i = 0; i < ports.size(); i++)
		if (isdigit(ports[i]) == 0)
			return (-1);
	return (1);
}

void	sig_handling(int sig)
{
	(void)sig;
	std::cout << "\b\bCONTROL+C HANDLED\n";
	stop = true;
}

int main(int ac, char**av)
{
	if (ac != 3){
		std::cout << "Error: Wrong number of arg" <<std::endl;
		std::cout << "Should be :./ircserv <port> <password>" <<std::endl;
		return 1;
	}
	if (parse(ac, av) == -1){
		std::cout << "Error: Wrong port" <<std::endl;
		std::cout << "Should be :./ircserv <port> <password>" <<std::endl;
		return 1;
	}
	stop = false;
	signal(SIGINT,sig_handling);
	try {
		Socket Sck(atoi(av[1]), std::string(av[2]));
		Sck.run();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
