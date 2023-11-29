#pragma once
#include "Socket.hpp"
class Server
{
private:
	std::map<int, std::string> map;
public:
	Server();
	void store_msgs(int socket, char *buf);
	~Server();
};
