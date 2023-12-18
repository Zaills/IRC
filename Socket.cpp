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

Socket::Socket(int port, std::string pass) : server(port, pass){
	this->opt = 1;
	this->addrlen = sizeof(address);
	if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("socket");
	if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("setsockopt");

	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(port);

	if (bind(this->server_fd, (struct sockaddr*)&this->address, sizeof(this->address)) < 0)
		throw std::runtime_error("bind");
}

 Socket::~Socket(){
	for(int i = 0; i <= this->max_fd; ++i){
		if(FD_ISSET(i, &this->master_set))
			close(i);
	}
}

void Socket::run(){
	char	buffer[1024] = {0};
	int		new_sd;
	fd_set	working_set;

	if (listen(this->server_fd, 32) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	FD_ZERO(&this->master_set);
	this->max_fd = this->server_fd;
	FD_SET(this->server_fd, &this->master_set);

	timeval	timeout;
	timeout.tv_sec = 800000;
	timeout.tv_usec = 0;
	int	rc, len, close_conn;
	int	desc_r;

	do {
		memcpy(&working_set, &this->master_set, sizeof(this->master_set));

		rc = select(this->max_fd + 1, &working_set, NULL, NULL, &timeout);
		if (rc < 0) {
			perror("select");
			break;
		}
		if (rc == 0){
			std::cout << "Select timed out." << std::endl;
			break;
		}
		desc_r = rc;
		for (int i = 0; i <= this->max_fd && desc_r > 0; ++i) {
			if (FD_ISSET(i, &working_set)) {
				desc_r -=1;
				if (i == this->server_fd) {
						if ((new_sd = accept(this->server_fd, NULL, NULL)) < 0) {
							perror("accept");
							stop = true;
							break;
						}
						server.addClient(new_sd);
						FD_SET(new_sd, &this->master_set);
						if (new_sd > this->max_fd)
							this->max_fd = new_sd;
				}
				else {
					close_conn = false;
					if ((rc = recv(i, buffer, sizeof(buffer), 0)) < 0){
						perror("recv");
						close_conn = true;
					}
					if (rc == 0){
						server.delChanelClient(i);
						server.delClient(i);
						close_conn = true;
					}
					if (!close_conn){
						len = rc;
						(void) len;
						server.get_msgs(i, buffer);
						memset(buffer,'\0',1024);
					}

					if (close_conn){
						close(i);
						FD_CLR(i, &this->master_set);
						if (i == this->max_fd){
							while (FD_ISSET(this->max_fd, &this->master_set) == false)
								this->max_fd -= 1;
						}
					}
				} // end of readable
			} // end of if FD_SET
		} // end of loop
	}while (stop == false);
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

Server	*Socket::get_server(){
	return &this->server;
}
