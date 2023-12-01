/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:20:13 by rvinour           #+#    #+#             */
/*   Updated: 2023/12/01 14:47:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server side C/C++ program to demonstrate Socket
// programming
#include "Socket.hpp"
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>

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
