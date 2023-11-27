/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rvinour <rvinour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:20:13 by rvinour           #+#    #+#             */
/*   Updated: 2023/11/27 16:04:22 by rvinour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Server side C/C++ program to demonstrate Socket
// programming
#include <algorithm>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(void)
{
	int server_fd, new_socket;
	ssize_t valread = 0;
	struct sockaddr_in address;
	int opt = 1;
	socklen_t addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	std::string hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	//send(new_socket, hello.c_str(), hello.size(), 0);
	while(valread != 1){
		valread = read(new_socket, buffer, 1024 - 1);
		std::cout << buffer << " " << valread << std::endl << std::endl;
		buffer[0] = 0;
	}

	close(new_socket);
	close(server_fd);
	return 0;
}
