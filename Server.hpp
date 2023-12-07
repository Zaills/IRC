#pragma once

#include "Client.hpp"
#include <map>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

class Server
{
private:
	Server(const Server&);
	Server operator=(const Server&);
	std::map<int, std::string> _client_msgs;
	std::map<int, client *> _clients;
	std::string _password;
	int _ports;
	int check_input(std::string str, int fd) const;
	int check_empty(std::string str) const;
	void setNick(int fd);
	void setUser(int fd);
	void setPass(int fd);
	void LoggedIn(int fd);
public:
	Server(int ports, std::string password);
	void get_msgs(int socket, char *buf);
	void addClient(int fd_clients);
	void delClient(int fd_clients);
	~Server();
};
