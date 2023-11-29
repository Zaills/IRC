#include "Server.hpp"

Server::Server()
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
	if (!this->map.count(socket))
		this->map.insert(std::pair<int,std::string>(socket,temp));
	else
		this->map.insert(std::pair<int,std::string>(socket, this->map.at(socket).append(temp)));
	if (temp.find("\n",0) == temp.size()-1)
	{
		std::cout << this->map.at(socket);
		this->map.erase(socket);
	}
}
