#pragma once

#include "Client.hpp"
#include "Chanel.hpp"
#include <map>
#include <vector>
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
	std::vector<Chanel *> _chanels;
	std::map<std::string, std::vector<std::string> > _invited;
	int _ports;

public:
	Server(int ports, std::string password);
	void get_msgs(int socket, char *buf);
	void addClient(int fd_clients);
	void delClient(int fd_clients);
	void delChanelClient(int fd);
	std::vector<Chanel *> *get_chanel();
	void new_chanel(Chanel *chanel);
	void addInvited(std::string key, std::string value);
	void delInvited(std::string key, std::string value);
	bool isInvited(std::string key, std::string value);

	~Server();
};
