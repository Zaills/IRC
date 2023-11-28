/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 16:59:01 by marvin            #+#    #+#             */
/*   Updated: 2023/11/28 16:59:01 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <algorithm>
# include <cstring>
# include <iostream>
# include <netinet/in.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <string>
# include <sys/socket.h>
# include <unistd.h>
# define PORT 8080

# include <exception>
# include <list>

class Socket{

private:
	struct sockaddr_in address;
	socklen_t	addrlen;
	int			server_fd;
	int 		opt;
	int			socket_client;

	Socket(const Socket&);
	Socket operator=(const Socket&);

public:
	Socket();
	~Socket();

	int			get_server_fd();
	sockaddr_in	get_address();
	socklen_t	get_addrlen();
	int			get_socket_client();

	void		set_socket_client(int);
};

#endif
