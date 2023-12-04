#include "Server.hpp"

Server::Server(int ports, std::string password) : _password(password),  _ports(ports)
{
	std::cout << "Server created\n";
}

Server::~Server()
{
	std::cout << "Server terminated\n";
}

void Server::get_msgs(int fd_client, char *buf) //ctrl+d 2 fois de suite casse tout
{
	std::string temp = buf;
	if (!this->_client_msgs.count(fd_client))
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client,temp));
	else
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client, this->_client_msgs.at(fd_client).append(temp)));
	if (logClients(fd_client) == -1)
		return ;
	if (temp.find("\n",0) == temp.size())
	{
		std::cout << "FINAL MESSAGE IS :"<< this->_client_msgs.at(fd_client);
		this->_client_msgs.erase(fd_client);
	}
}

int Server::logClients(int fd_client)
{
	client *ptr = this->_clients.at(fd_client); //probleme avec : "enter" en input et CTRL D
	if (ptr->is_logged == false)
	{
		std::string msgs = this->_client_msgs.at(fd_client);
		if (ptr->nick.empty() == true && msgs.empty() == false)
		{
			if (msgs[0] == '\n' && msgs.size() == 1)
			{
				this->_client_msgs.at(fd_client).clear();
				return -1;
			}
			ptr->nick = msgs.substr(0,msgs.find("\n")); //check for multi name
			this->_client_msgs.at(fd_client).clear();
			send(fd_client, "Waiting for username :\n",24,0);
		}
		else if (ptr->user.empty() == true && msgs.empty() == false && ptr->nick.empty() == false)
		{
			if (msgs[0] == '\n' && msgs.size() == 1)
			{
				this->_client_msgs.at(fd_client).clear();
				return -1;
			}
			ptr->user = msgs.substr(msgs.find("\n") + 1,msgs.size() - msgs.find("\n") - 2);
			send(fd_client, "SUCCESFULLY LOGGED IN\n",23,0);
			this->_client_msgs.at(fd_client).clear();
		}
		else
			return -1;
		if (ptr->user.empty() == false && ptr->nick.empty() == false)
			ptr->is_logged = true;
		else
			return -1;
	}
	std::cout << "USER : " << ptr->user << " with nick : " << ptr->nick << " is logged : " << ptr->is_logged << std::endl;
	return 1;
}

void Server::addClient(int fd_client)
{
	if (!this->_clients.count(fd_client))
	{
		this->_clients.insert(std::pair<int, client*>(fd_client,new client));
		this->_clients.at(fd_client)->is_logged = false;
		send(fd_client, "Waiting for nickname :\n",24,0);
	}
}

void Server::delClient(int fd_client)
{
	if (this->_clients.count(fd_client))
	{
		this->_client_msgs.at(fd_client).clear();
		delete this->_clients.at(fd_client);
		this->_clients.erase(fd_client);
		this->_client_msgs.erase(fd_client);
	}
}


