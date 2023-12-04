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

Socket::Socket() : server(PORT, std::string("temp")){
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

	//init master fd
	FD_ZERO(&this->master_set);
	this->max_fd = this->server_fd;
	FD_SET(this->server_fd, &this->master_set);

	//time before stop select
	timeval	timeout;
	timeout.tv_sec = 60;
	timeout.tv_usec = 0;

	int	rc, len, close_conn;
	int	desc_r, stop = false;
	do {
		//seting the fd I want to read
		memcpy(&working_set, &this->master_set, sizeof(this->master_set));

		//select on working_set
		std::cout << std::endl << "Waiting on select..." << std::endl;
		rc = select(this->max_fd + 1, &working_set, NULL, NULL, &timeout);
		if (rc < 0) {
			perror("select");
			break;
		}
		if (rc == 0){
			std::cout << "Select timed out." << std::endl;
			break;
		}

		//looking witch fd we need to read
		desc_r = rc;
		for (int i = 0; i <= this->max_fd && desc_r > 0; ++i) {
			if (FD_ISSET(i, &working_set)) {
				desc_r -=1;

				//if it's the server fd -> new connection
				if (i == this->server_fd) {
					std::cout << "Server Socket is readable" << std::endl;
						if ((new_sd = accept(this->server_fd, NULL, NULL)) < 0) {
							perror("accept");
							stop = true;
							break;
						}
						std::cout << "New connection: " << new_sd << std::endl;
						server.addClient(new_sd); // ajoute le client dans la map<fd, struct>
						FD_SET(new_sd, &this->master_set);
						if (new_sd > this->max_fd)
							this->max_fd = new_sd;
				}

				//if not then it's readable
				else {
					std::cout << "Socket is readable" << std::endl;
					close_conn = false;
					if ((rc = recv(i, buffer, sizeof(buffer), 0)) < 0){
						perror("recv");
						close_conn = true;
					}
					if (rc == 0){ //retire de struct
						std::cout << "Connection ended" << std::endl;
						close_conn = true;
					}
					if (!close_conn){
						//echo back to client
						len = rc;
						std::cout << "recieved: " << len << std::endl;
						server.store_msgs(new_sd, buffer); //parsing message (for login and commands)
/* 						if ((rc = send(i, buffer, len, 0)) < 0){  //commented out for login test
							perror("send");
							close_conn = true;
						} */
						memset(buffer,'\0',1024); //clearing the buffer msgs
					}

					//if flag close_conn we need to clean up
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

