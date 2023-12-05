/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gouz <gouz@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:20:13 by rvinour           #+#    #+#             */
/*   Updated: 2023/12/01 18:24:39 by gouz             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server side C/C++ program to demonstrate Socket
// programming
#include "Socket.hpp"
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include "Server.hpp"

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

int main(void)
{
	try {
		Socket Sck;
		Sck.run();
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
