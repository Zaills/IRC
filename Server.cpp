#include "Server.hpp"

Server::Server(int ports, std::string password) : _password(password),  _ports(ports)
{
	std::cout << "Server created\n";
}

Server::~Server()
{
	std::cout << "Server terminated\n";
}

void Server::store_msgs(int fd_client, char *buf) //ctrl+d 2 fois de suite casse tout
{
	std::string temp = buf;
	std::cout << "RECEIVED TO SERVER : \n" << buf << std::endl;
	if (!this->_client_msgs.count(fd_client))
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client,temp));
	else
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client, this->_client_msgs.at(fd_client).append(temp)));
	if (logClients(fd_client) == -1) //make the client logged
		return ;
	if (temp.find("\n",0) == temp.size() - 1)
	{
		std::cout << "FINAL MESSAGE IS :"<< this->_client_msgs.at(fd_client);
		this->_client_msgs.erase(fd_client);
	}

}

int Server::logClients(int fd_client)
{
	client ptr = this->_clients.at(fd_client);
	if (ptr.is_logged == false)
	{
		std::string msgs = this->_client_msgs.at(fd_client);
		if (ptr.nick.empty() == true && msgs.empty() == false)
		{
			ptr.nick == msgs.substr(0,msgs.find("\n"));
			msgs.erase(0,msgs.find("\n"));
		}
		else
		{
			send(fd_client, "Waiting for nickname :",23,0);
			return -1;
		}
		if (ptr.user.empty() == true && msgs.empty() == false)
		{
			ptr.user == msgs.substr(0,msgs.find("\n"));
			msgs.erase(0,msgs.find("\n"));
		}
		else
			return -1;
		if (ptr.user.empty() == false && ptr.nick.empty() == false)
			ptr.is_logged = true;
		else
			return -1;
	}
	return 1;
}
