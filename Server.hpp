#pragma once
#include "Socket.hpp"
#include "Client.hpp"
#include <map>

class Server
{
private:
	Server(const Server&);
	Server operator=(const Server&);
	std::map<int, std::string> _client_msgs;
	std::map<int, client> _clients;
	std::string _password;
	int _ports;
	int logClients(int fd);
public:
	Server(int ports, std::string password);
	void store_msgs(int socket, char *buf);
	~Server();
};
