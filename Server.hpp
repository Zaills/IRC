#pragma once
#include "Socket.hpp"
class Server
{
public:
	Server(int ports, std::string password);
	void store_msgs(int socket, char *buf);
	~Server();
private:
	Server(const Server&);
	Server operator=(const Server&);
	std::map<int, std::string> _client_msgs;
	std::string _password;
	int _ports;
};
