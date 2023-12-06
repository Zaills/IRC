#include "Server.hpp"

Server::Server(int ports, std::string password) : _password(password),  _ports(ports)
{
	std::cout << "Server created\n";
}

Server::~Server()
{
	std::cout << "Server terminated\n";
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
	if (temp[0] == '\n')
		return;
	if (!this->_client_msgs.count(fd_client))
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client,temp));
	else
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client, this->_client_msgs.at(fd_client).append(temp)));
	std::cout << "MSG:" << this->_client_msgs[fd_client];
	std::string cmd[] = {"NICK ", "USER ", "PASS ", "INVITE ", "TOPIC ", "MODE ", "KICK "};
	int id;
	while (this->_client_msgs.at(fd_client).empty() == false && this->_client_msgs.at(fd_client).find('\n') != std::string::npos)
	{
		id = 999;
		for (int i = 0; i < 7; i++)
		{
			if (this->_client_msgs[fd_client].substr(0,cmd[i].size()) == cmd[i])
			{
				id = i;
				break;
			}
		}
		switch (id)
		{
		case 0:
			setNick(fd_client);
			break;
		case 1:
			setUser(fd_client);
			break;
		case 2:
			setPass(fd_client);
			break;
		case 3:
			//INVITE
			break;
		case 4:
			//TOPIC
			break;
		case 5:
			//MODE
			break;
		case 6:
			//KICK
			break;
		default:
			this->_client_msgs[fd_client].clear();
			break;
		}
		std::cout << "HERE\n";
	}
}

void Server::setNick(int fd)
{
	client *ptr = this->_clients.at(fd);
	std::string *msgs = &(this->_client_msgs.at(fd));
	if (ptr->nick.empty() == false)
	{
		msgs->clear();
		return;
	}
	ptr->nick = msgs->substr(5, msgs->find('\n')-5);
	msgs->erase(0, msgs->find('\n')+1);
	std::cout << "NICK :" << ptr->nick << std::endl;
	LoggedIn(fd);
}

void Server::setUser(int fd)
{
	client *ptr = this->_clients.at(fd);
	std::string *msgs = &(this->_client_msgs.at(fd));
	if (ptr->user.empty() == false)
	{
		msgs->clear();
		return;
	}
	ptr->user = msgs->substr(5, msgs->find("*")-8);
	std::cout << "USER :" << ptr->user << std::endl;
	msgs->erase(0, msgs->find("\n")+1);
	LoggedIn(fd);
}

void Server::setPass(int fd)
{
	client *ptr = this->_clients.at(fd);
	std::string *msgs = &(this->_client_msgs.at(fd));
	if (ptr->password.empty() == false)
	{
		msgs->clear();
		return;
	}
	ptr->password = msgs->substr(5, msgs->find('\n')-5);
	std::cout << "PASS :" << ptr->password << std::endl;
	msgs->erase(0, msgs->find('\n')+1);
	LoggedIn(fd);
}

void Server::LoggedIn(int fd)
{
	client *ptr = this->_clients.at(fd);
	if (!ptr->nick.empty() && !ptr->user.empty())
	{
		ptr->is_logged = true;
		std::cout << "A CLIENT HAS BEEN CONNECTED !\n";
	}
}

void Server::addClient(int fd_client)
{
	if (!this->_clients.count(fd_client))
	{
		this->_clients.insert(std::pair<int, client*>(fd_client,new client));
		this->_clients.at(fd_client)->is_logged = false;
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

