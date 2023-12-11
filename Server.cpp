#include "Server.hpp"
#include "Socket.hpp"
#include "cmd/CMD.hpp"


Server::Server(int ports, std::string password) : _password(password),  _ports(ports)
{
	std::cout << "Server created\n";
}

Server::~Server()
{
	std::cout << "Server terminated correctly\n";
	std::map<int, client*>::const_iterator it = this->_clients.begin();
	while (it != this->_clients.end()){
		if ((*it).second != NULL)
			delete (*it).second;
		it++;
	}
}

int Server::check_input(std::string msgs) const
{
	if (msgs[0] == '\n' && msgs.size() == 1)
		return -1;
	std::string str = &msgs[5];
	if (str.find('\r') == std::string::npos)
		str = str.substr(0,msgs.find('\n')-5);
	else
		str = str.substr(0,msgs.find('\r')-5);
	std::map<int, client*>::const_iterator it = this->_clients.begin();
	while (it != this->_clients.end()){
		if ((*it).second->nick == str)
			return -1;
		it++;
	}
	return 1;
}

int Server::check_empty(std::string msgs) const
{
	std::string temp = &msgs[3];
	if (temp.size() == 1)
		return -1;
	temp = &msgs[4];
	for(size_t i = 0; i < temp.size(); i++)
		if (!isspace(temp[i]))
			return 1;
	return -1;
}

void Server::setNick(int fd) //432 ERRONEUSNICKNAME ????? que faire
{
	client *ptr = this->_clients.at(fd);
	std::string *msgs = &(this->_client_msgs.at(fd));
	if (ptr->password != this->_password)
		return ;
	else if (ptr->nick.empty() == false)
	{
		std::string buf = ": 463 " + ptr->nick + " ERR_ALREADYREGISTRED :You may not reregister\n";
		send(fd, buf.c_str(), buf.size(),0);
		return ;
	}
	else if (check_input((*msgs)) == -1)
	{
		std::string buf = ": 433 " + ptr->nick + " ERR_NICKNAMEINUSE :Nickname is already in use\n";
		send(fd, buf.c_str(), buf.size(), 0);
		return ;
	}
	else if (check_empty((*msgs)) == -1)
	{
		std::string buf = ": 463 " + ptr->nick + " ERR_NONICKNAMEGIVEN :No nickname given\n";
		send(fd, buf.c_str(), buf.size(), 0);
		return ;
	}
	else
	{
		for (size_t i = 5; i < msgs->size(); i++)
		{
			if (i == 5)
			{
				if (!isalpha((*msgs)[5]))
				{
					std::string buf = ": 432 " + ptr->nick + " ERR_ERRONEUSNICKNAME :Erroneus nickname\n";
					send(fd, buf.c_str(), buf.size(),0);
					return ;
				}
			}
			/* if (!isalnum((*msgs)[i]) && (*msgs)[i] != '[' && (*msgs)[i] != ']' && (*msgs)[i] != '-' &&
				(*msgs)[i] != '{' && (*msgs)[i] != '}' && (*msgs)[i] != '\\' && (*msgs)[i] != '^' && (*msgs)[i] != '`')
			{
				std::string buf = ": 432 " + ptr->nick + " ERR_ERRONEUSNICKNAME :Erroneus nickname\n";
				send(fd, buf.c_str(), buf.size(),0);
				msgs->erase(0, msgs->find('\n')+1);
				return ;
			} */
		}
	}
	if (msgs->find('\r') == std::string::npos)
		ptr->nick = msgs->substr(5, msgs->find('\n')-5);
	else
		ptr->nick = msgs->substr(5, msgs->find('\r')-5);
	std::cout << "NICK :" << ptr->nick<<std::endl;
	LoggedIn(fd);
}

void Server::setUser(int fd)
{
	client *ptr = this->_clients.at(fd);
	std::string *msgs = &(this->_client_msgs.at(fd));
	if (ptr->password != this->_password)
		return ;
	if (ptr->user.empty() == false)
	{
		std::string buf = ": 462 " + ptr->nick + " ERR_ALREADYREGISTRED :You may not reregister\n";
		send(fd, buf.c_str(), buf.size(), 0);
		return;
	}
	else if (check_empty((*msgs)) == -1)
	{
		std::string buf = ": 461 " + ptr->nick + " ERR_NEEDMOREPARAMS :Not enough parameters\n";
		send(fd,buf.c_str(), buf.size(), 0);
		return ;
	}
	if (msgs->find('\r') == std::string::npos)
		ptr->user = msgs->substr(5, msgs->find('\n')-5);
	else
		ptr->user = msgs->substr(5, msgs->find("*")-8);
	std::cout << "USER :" << ptr->user << std::endl;
	LoggedIn(fd);
}

void Server::setPass(int fd)
{
	client *ptr = this->_clients.at(fd);
	std::string *msgs = &(this->_client_msgs.at(fd));
	if (ptr->password.empty() == false)
	{
		std::string buf = ": 462 " + ptr->nick + " ERR_ALREADYREGISTRED :You may not reregister\n";
		send(fd, buf.c_str(), buf.size(), 0);
		return;
	}
	if (check_empty((*msgs)) == -1)
	{
		std::string buf = ": 461 " + ptr->nick + " ERR_NEEDMOREPARAMS :Not enough parameters\n";
		send(fd, buf.c_str(), buf.size(), 0);
		return ;
	}
	if (msgs->find('\r') == std::string::npos)
	{
		if (msgs->substr(5, msgs->find('\n')-5) == this->_password)
		{
			ptr->password = msgs->substr(5, msgs->find('\n')-5);
			std::cout << "(nc)PASS :" << ptr->password << std::endl;
			LoggedIn(fd);
		}
		return ;
	}
	if (msgs->substr(5, msgs->find('\n')-6) == this->_password)
	{
		ptr->password = msgs->substr(5, msgs->find('\n')-6);
		std::cout << "(hexchat)PASS :" << ptr->password << std::endl;
		LoggedIn(fd);
	}
	else
	{
		std::string buf = ": 464 " + ptr->nick + " ERR_PASSWDMISMATCH :Password incorrect\n";
		send(fd, buf.c_str(), buf.size(), 0);
	}
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


//		FONCTION PUBLIQUE		//

void Server::get_msgs(int fd_client, char *buf)
{
	std::string temp = buf;
	if (temp[0] == '\n')
		return;
	if (!this->_client_msgs.count(fd_client))
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client,temp));
	else
		this->_client_msgs.insert(std::pair<int,std::string>(fd_client, this->_client_msgs.at(fd_client).append(temp)));
	std::cout << "MSG:" << this->_client_msgs[fd_client];
	std::string cmd[] = {"NICK ", "USER ", "PASS ", "INVITE ", "TOPIC ", "MODE ", "KICK ", "JOIN "};
	int id;
	while (this->_client_msgs.at(fd_client).empty() == false && this->_client_msgs.at(fd_client).find('\n') != std::string::npos)
	{
		id = 999;
		for (int i = 0; i < 8; i++)
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
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 1:
			setUser(fd_client);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 2:
			setPass(fd_client);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 3:
			//INVITE
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 4:
			//TOPIC
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 5:
			//MODE
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 6:
			cmd_kick(this->_client_msgs[fd_client].erase(0, cmd[7].size()), this->_clients[fd_client], this);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 7:
			cmd_join(this->_client_msgs[fd_client].erase(0, cmd[7].size()), this->_clients[fd_client], this);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 8:
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		default:
			this->_client_msgs[fd_client].erase(0,this->_client_msgs[fd_client].find('\n')+1);
			break;
		}
	}
}

void Server::addClient(int fd_client)
{
	if (this->_clients.count(fd_client) == 0)
	{
		std::cout << "A CLIENT HAS BEEN ADDED :" << fd_client << "\n";
		this->_clients.insert(std::pair<int, client*>(fd_client,new client));
		this->_clients.at(fd_client)->is_logged = false;
		this->_clients.at(fd_client)->fd = fd_client;
	}
}

void Server::delClient(int fd_client)
{
	if (this->_clients.count(fd_client) == 1)
	{
		std::cout << "A CLIENT HAS BEEN DELETED :" << fd_client << "\n";
		this->_client_msgs.erase(fd_client);
		delete this->_clients.at(fd_client);
		this->_clients.erase(fd_client);
	}
}

std::vector<Chanel *> *Server::get_chanel(){
	return &this->_chanels;
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
