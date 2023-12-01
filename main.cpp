/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:20:13 by rvinour           #+#    #+#             */
/*   Updated: 2023/12/01 13:42:59 by marvin           ###   ########.fr       */
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
	Socket Sck;

	/*ssize_t valread = 0;
	int server_fd = Sck.get_server_fd();
	struct sockaddr_in address = Sck.get_address();
	socklen_t addrlen = Sck.get_addrlen();


	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	int new_socket, s2 = 0;
	char buffer[1024] = {0};
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	Sck.set_socket_client(new_socket);

	//send(new_socket, hello.c_str(), hello.size(), 0);
	while(valread != 1){
		valread = recv(new_socket, buffer, 1024 - 1, 0);
		std::cout << buffer << " " << valread << std::endl << std::endl;
		if (valread == 0)
			exit(0);
		for (long i = 0; i < valread && i < 1024; i++)
			buffer[i] = 0;
		if(!s2 && (s2 = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0){
			perror("accept");
			exit(EXIT_FAILURE);
		}
		else if (s2) {
			std::cout << "ns connected" << std::endl;
		}
	}

	if (s2)
		close(s2);*/

	Sck.run();
	return 0;
}
