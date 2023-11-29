/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmorandi <nmorandi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:20:13 by rvinour           #+#    #+#             */
/*   Updated: 2023/11/29 17:13:32 by nmorandi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server side C/C++ program to demonstrate Socket
// programming
#include "Socket.hpp"
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

int main(int ac, char **av)
{
	//if (parse(ac, av) == -1) { exit(EXIT_FAILURE); }
	//Server serv(atoi(av[1]),std::string(av[2]));
	Server serv(8080,"oui");
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
		serv.store_msgs(new_socket,buffer); //need to handle for hexchat
		std::cout << buffer << " " << valread << std::endl << std::endl;
		for (long i = 0; i < valread && i < 1024; i++)
			buffer[i] = 0;
	}

	return 0;
}
