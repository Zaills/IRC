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
	// for(i = 0; i <= max_fd; ++i){
	// 	if(FD_ISSET(i, &master_set))
	// 		close(i);
	// }

	// if (this->server_fd)
	// 	close(this->server_fd);
	// std::for_each(this->socket_client.begin(), this->socket_client.end(), close);
}

void Socket::run(){
	char	buffer[1024] = {0};
	int		max_fd, new_sd;
	fd_set	master_set, working_set;

	if (listen(this->server_fd, 32) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	//init master fd
	FD_ZERO(&master_set);
	max_fd = this->server_fd;
	FD_SET(this->server_fd, &master_set);

	//time before stop select
	struct	timeval	timeout;
	timeout.tv_sec = 60;
	timeout.tv_usec = 0;

	int	rc, len, close_conn;
	int	desc_r, stop = false;
	do {
		//seting the fd I want to read
		memcpy(&working_set, &master_set, sizeof(master_set));

		//select on working_set
		std::cout << "max_fd: " << max_fd << std::endl;
		std::cout << "Waiting on select..." << std::endl;
		rc = select(max_fd + 1, &working_set, NULL, NULL, &timeout);
		if (rc < 0) {
			perror("select");
			break;
		}
		if (rc == 0){
			std::cout << "Select timed out." << std::endl;
			break;
		}

		//loking witch fd we need to read
		desc_r = rc;
		for (int i = 0; i<=max_fd && desc_r > 0; ++i) {

			if (FD_ISSET(i, &working_set)) {
				desc_r -=1;

				//if it's the server fd -> new connection
				if (i == this->server_fd) {
					std::cout << "Server Socket is readable" << std::endl;
					do {
						if ((new_sd = accept(this->server_fd, NULL, NULL)) < 0) {
							if (errno != EWOULDBLOCK) {
								perror("accept");
								stop = true;
							}
							break;
						}
						std::cout << "New connection: " << new_sd << std::endl;
						FD_SET(new_sd, &master_set);
						if (new_sd > max_fd)
							max_fd = new_sd;
					}while (new_sd != -1);
				}

				//if not then it's readable
				else {
					std::cout << "Socket is readable" << std::endl;
					close_conn = false;
					do {
						if ((rc = recv(i, buffer, sizeof(buffer), 0)) < 0){
							perror("recv");
							close_conn = true;
							break;
						}
						if (rc == 0){
							std::cout << "Connection ended" << std::endl;
							close_conn = true;
							break;
						}

						//echo back to client
						len = rc;
						std::cout << "recieved: " << len << std::endl;
						if ((rc = send(i, buffer, len, 0)) < 0){
							perror("send");
							close_conn = true;
							break;
						}
					}while (true);

					//if flag close_conn we need to clean up
					if (close_conn){
						close(i);
						FD_CLR(i, &master_set);
						if (i == max_fd){
							while (FD_ISSET(max_fd, &master_set) == false)
								max_fd -= 1;
						}
					}
				} // end of readable

			} // end of if FD_SET
		} // end of loop
	}while (stop == false);

	for(int i = 0; i <= max_fd; ++i){
		if(FD_ISSET(i, &master_set))
			close(i);
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

