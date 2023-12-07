#include "Server.hpp"
#include "Socket.hpp"

Server::Server(int ports, std::string password) : _password(password),  _ports(ports)
{
	std::cout << "Server created\n";
}

Server::~Server()
{
	std::cout << "Server terminated correctly\n";
	std::map<int, client*>::const_iterator it = this->_clients.begin();
	while (it != this->_clients.end()){
		if ((*it).second != NULL) {
			delete (*it).second;
		}
		it++;
	}
}

int Server::check_input(std::string msgs, int fd) const
{
	if (msgs[0] == '\n' && msgs.size() == 1)
		return -1;
	std::string str = &msgs[5];
	str = str.erase(str.size()-1,str.size());
	std::map<int, client*>::const_iterator it = this->_clients.begin();
	while (it != this->_clients.end()){
		if ((*it).second->nick == str && (*it).first != fd){
			return -1;
		}
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
	{
		if (!isspace(temp[i]))
			return 1;
	}
	return -1;
}

void Server::setNick(int fd) //432 ERRONEUSNICKNAME ????? que faire
{
	client *ptr = this->_clients.at(fd);
	std::string *msgs = &(this->_client_msgs.at(fd));
	if (ptr->password != this->_password)
	{
		msgs->erase(0, msgs->find('\n')+1);
		return ;
	}
	else if (ptr->nick.empty() == false)
	{
		send(fd, "462 ERR_ALREADYREGISTRED :You may not register\n",48,0);
		msgs->erase(0, msgs->find("\n")+1);
		return ;
	}
	else if (check_input((*msgs), fd) == -1)
	{
		std::string buf = "433 ERR_NICKNAMEINUSE\n" + ptr->nick + " :Nickname is already in use\n";
		send(fd, buf.c_str(), buf.size(),0);
		msgs->erase(0, msgs->find('\n')+1);
		return ;
	}
	else if (check_empty((*msgs)) == -1)
	{
		msgs->erase(0, msgs->find('\n')+1);
		send(fd, "431 ERR_NONICKNAMEGIVEN\n :No nickname given\n",45,0);
		return ;
	}
/* 	else{
		for (size_t i = 4; i < msgs->size(); i++)
			if ((*msgs)[i] > 127 || (*msgs)[i] < 0){ --> compile error data type
				std::string buf = "432 ERR_ERRONEUSNICKNAME\n" + ptr->nick + " :Erroneus nickname\n";
				send(fd, buf.c_str(), buf.size(),0);
				msgs->erase(0, msgs->find('\n')+1);
				return ;
			}
	} */
	ptr->nick = msgs->substr(5, msgs->find('\n')-5);
	msgs->erase(0, msgs->find('\n')+1);
	std::cout << "NICK :" << ptr->nick << std::endl;
	LoggedIn(fd);
}

void Server::setUser(int fd)
{

	client *ptr = this->_clients.at(fd);
	std::string *msgs = &(this->_client_msgs.at(fd));
	if (ptr->password != this->_password)
	{
		//std::cout << ptr->password << " == " << this->_password << "\n";
		return ;
	}
	if (ptr->user.empty() == false)
	{
		send(fd, "462 ERR_ALREADYREGISTRED :You may not register\n",48,0);
		msgs->erase(0, msgs->find("\n")+1);
		return;
	}
	else if (check_empty((*msgs)) == -1)
	{
		send(fd,"461 ERR_NEEDMOREPARAMS\nUSER :Not enough parameters\n",52,0);
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
		send(fd, "462 ERR_ALREADYREGISTRED :You may not reregister\n",48,0);
		msgs->erase(0, msgs->find('\n')+1);
		return;
	}
	if (check_empty((*msgs)) == -1)
	{
		send(fd,"461 ERR_NEEDMOREPARAMS\nPASS :Not enough parameters\n",52,0);
		msgs->erase(0, msgs->find('\n')+1);
		return ;
	}
	std::cout << msgs->substr(5, msgs->find('\n')-6) << " == " << this->_password << "\n";
	if (msgs->substr(5, msgs->find('\n')-6) == this->_password)
	{
		ptr->password = msgs->substr(5, msgs->find('\n')-6);
		std::cout << "PASS :" << ptr->password << std::endl;
		LoggedIn(fd);
		msgs->erase(0, msgs->find('\n')+1);
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

