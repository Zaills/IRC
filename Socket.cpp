/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 17:04:42 by marvin            #+#    #+#             */
/*   Updated: 2023/11/28 17:04:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"
#include <algorithm>
#include <asm-generic/socket.h>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <sys/select.h>
#include <sys/socket.h>

Socket::Socket(){
	this->opt = 1;
	this->addrlen = sizeof(address);
	if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("socket");
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		throw std::runtime_error("setsockopt");

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(PORT);

	if (bind(this->server_fd, (struct sockaddr*)&this->address, sizeof(this->address)) < 0)
		throw std::runtime_error("bind");
}

Socket::~Socket(){
	if (this->server_fd)
		close(this->server_fd);
	std::for_each(this->socket_client.begin(), this->socket_client.end(), close);
}

void Socket::run(){
	bool	stop = false;
	char	buffer[1024] = {0};

	if (listen(this->server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	int		new_socket;
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	socket_client.push_back(new_socket);
	std::list<int>::iterator it = this->socket_client.begin();

	int		retval;
	while (stop) {
		//retval = select(FD_SETSIZE, &(fd_set) new_socket, NULL, NULL, tv)

	}
}

int Socket::get_server_fd(){
	return this->server_fd;
}

sockaddr_in Socket::get_address(){
	return this->address;
}

socklen_t Socket::get_addrlen(){
	return this->addrlen;
}

int Socket::get_socket_client(){
	return this->socket_client.front();
}

void Socket::set_socket_client(int sc){
	this->socket_client.push_front(sc);
}

