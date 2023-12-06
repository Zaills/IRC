/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gouz <gouz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:20:13 by rvinour           #+#    #+#             */
/*   Updated: 2023/12/06 14:01:19 by gouz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server side C/C++ program to demonstrate Socket
// programming
#include "Socket.hpp"
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include "Server.hpp"
#include <signal.h>

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
	std::cout << "HELLO LE C\n";
}

int main(void)
{
	signal(SIGINT,sig_handling);
	try {
		Socket Sck;
		Sck.run();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
