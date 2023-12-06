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
	if (temp.find("\n",0) == temp.size() - 1) //this is for handling CTRL+D
	{
		std::cout << "FINAL MESSAGE IS :"<< this->_client_msgs.at(fd_client);
		if (logClients(fd_client) == -1)
			return ;
		this->_client_msgs.erase(fd_client);
	}
	else
		return ;
}

int Server::logClients(int fd_client)
{
	client *ptr = this->_clients.at(fd_client);
	if (ptr->is_logged == false)
	{
		std::string *msgs = &(this->_client_msgs.at(fd_client));
		if (ptr->nick.empty() == true && msgs->empty() == false)
		{
			if (check_input(msgs, false) == -1)
				return -1;
			ptr->nick = msgs->substr(0,msgs->find("\n")); //check for multi name
			msgs->clear();
			send(fd_client, "Waiting for username :\n",24,0);
		}
		else if (ptr->user.empty() == true && msgs->empty() == false && ptr->nick.empty() == false)
		{
			if (check_input(msgs, true) == -1)
				return -1;
			ptr->user = msgs->substr(0,msgs->find("\n"));
			send(fd_client, "SUCCESFULLY LOGGED IN\n",23,0);
			msgs->clear();
			ptr->is_logged = true;
		}
		else
			return -1;
		if (ptr->is_logged == false)
			return -1;
	}
	std::cout << "USER : " << ptr->user << " with nick : " << ptr->nick << " is logged : " << ptr->is_logged << std::endl;
	return 1;
}

int Server::check_input(std::string *msgs, bool check_user) const
{
	if ((*msgs)[0] == '\n' && msgs->size() == 1)
	{
		msgs->clear();
		return -1;
	}
	std::string str = (*msgs);
	str.erase(str.size()-1);
	std::map<int, client*>::const_iterator it = this->_clients.begin();
	if (check_user == true){
		while (it != this->_clients.end()){
			if ((*it).second->user == str){
				(*msgs).clear();
				std::cout << "USER ALREADY EXIST\n";
				return -1;
			}
			it++;
		}
	}
	else{
		while (it != this->_clients.end()){
			if ((*it).second->nick == str){
				std::cout << "NICK ALREADY EXIST\n";
				(*msgs).clear();
				return -1;
			}
			it++;
		}
	}
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

std::vector<Chanel *> Server::get_chanel(){
	return this->_chanels;
}

void	Server::new_chanel(Chanel *chanel){
	this->_chanels.push_back(chanel);
}

int	Server::get_fd(std::string user){
	for (std::map<int, client*>::const_iterator it = this->_clients.begin(); it != this->_clients.end(); it++){
		if (it->second->user == user)
			return it->first;
	}
	return 0;
}
