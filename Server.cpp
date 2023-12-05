#include "Server.hpp"

Server::Server(int ports, std::string password) : _password(password),  _ports(ports)
{
	std::cout << "Server created\n";
}

Server::~Server()
{
	std::cout << "Server terminated\n";
}

int Server::logClients(int fd_client)
{
	//std::cout << "CLIENT IS :"<<fd_client << "\n";
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

//		FONCTION PUBLIQUE		//

void Server::get_msgs(int fd_client, char *buf) //ctrl+d 2 fois de suite casse tout
{
	std::string temp = buf;
	if (temp.size() == 1)
		return;
	if (!this->_client_msgs.count(fd_client)) // stock les msgs
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client,temp));
	else
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client, this->_client_msgs.at(fd_client).append(temp)));

	std::string cmd[] = {"NICK", "USER", "KICK", "INVITE", "TOPIC", "MODE"}; //check si une cmds
	int id = 999;
	for (int i = 0; i < 6; i++)
	{
		if (this->_client_msgs[fd_client].substr(0,cmd[i].size()) == cmd[i])
		{
			std::cout << "cmd find :" << cmd[i] << std::endl;
			id = i;
			break;
		}
	}
	if (id != 999) //modifie le buffer selon la cmd (pour recuper les args seulement)
		this->_client_msgs[fd_client]= &this->_client_msgs[fd_client][cmd[id].size() + 1];
	if (id < 2){
		if ((id == 1 && this->_clients.at(fd_client)->nick.empty() == true)
			|| (this->_clients.at(fd_client)->is_logged == true))  // CMD NICK en premier || peut pas se log si deja log
		{
			this->_client_msgs.at(fd_client).clear();
			return ;
		}
		std::cout << "FINAL MESSAGE IS :"<< this->_client_msgs.at(fd_client);
		if (logClients(fd_client) == -1)
			return ;
		this->_client_msgs.at(fd_client).clear();
	}
	if (this->_clients.at(fd_client)->is_logged == false)
		this->_client_msgs.at(fd_client).clear();
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

