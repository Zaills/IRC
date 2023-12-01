#include "Server.hpp"

Server::Server(int ports, std::string password) : _password(password),  _ports(ports)
{
	std::cout << "Server created\n";
}

Server::~Server()
{
	std::cout << "Server terminated\n";
}

void Server::store_msgs(int socket, char *buf) //ctrl+d 2 fois de suite casse tout
{
	std::string temp = buf;
	if (!this->_map.count(socket))
		this->_map.insert(std::pair<int,std::string>(socket,temp));
	else
		this->_map.insert(std::pair<int,std::string>(socket, this->_map.at(socket).append(temp)));
	if (temp.find("\n",0) == temp.size() - 1)
	{
		std::cout << this->_map.at(socket);
		this->_map.erase(socket);
	}
}
