/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmorandi <nmorandi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:20:13 by rvinour           #+#    #+#             */
/*   Updated: 2023/11/29 16:07:48 by nmorandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server side C/C++ program to demonstrate Socket
// programming
#include "Socket.hpp"
#include "Server.hpp"

int main(void)
{
	Server serv;

	ssize_t valread = 0;

	Socket Sck;
	int server_fd = Sck.get_server_fd();
	struct sockaddr_in address = Sck.get_address();
	socklen_t addrlen = Sck.get_addrlen();


	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	int new_socket = 0;
	char buffer[1024] = {0};
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	Sck.set_socket_client(new_socket);

	//send(new_socket, hello.c_str(), hello.size(), 0);
	while(valread != 1){
		valread = read(new_socket, buffer, 1024 - 1);
		serv.store_msgs(new_socket,buffer);
		std::cout << buffer << " " << valread << std::endl << std::endl;
		for (long i = 0; i < valread && i < 1024; i++)
			buffer[i] = 0;
	}

	return 0;
}
