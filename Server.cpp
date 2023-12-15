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
	std::vector<Chanel *>::const_iterator it2 = this->_chanels.begin();
	while (it2 != this->_chanels.end()){
		if (*it2 != NULL)
			delete (*it2);
		it2++;
	}

}

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
	std::string cmd[] = {"NICK ", "USER ", "PASS ", "INVITE ", "TOPIC ", "MODE ", "KICK ", "JOIN ", "PRIVMSG "};
	int id;
	while (this->_client_msgs.at(fd_client).empty() == false && this->_client_msgs.at(fd_client).find('\n') != std::string::npos)
	{
		id = 999;
		for (int i = 0; i < 9; i++)
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
			setNick(fd_client, this->_clients, this->_client_msgs, this->_password);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 1:
			setUser(fd_client, this->_clients, this->_client_msgs, this->_password);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 2:
			setPass(fd_client, this->_clients, this->_client_msgs, this->_password);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 3:
			cmd_invite(this->_client_msgs[fd_client].erase(0, cmd[3].size()), this->_clients[fd_client], this);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 4:
			cmd_topic(this->_client_msgs[fd_client].erase(0, cmd[4].size()), this->_clients[fd_client], this);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 5:
			cmd_mode(this->_client_msgs[fd_client].erase(0, cmd[5].size()), this->_clients[fd_client], this);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 6:
			cmd_kick(this->_client_msgs[fd_client].erase(0, cmd[6].size()), this->_clients[fd_client], this);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 7:
			cmd_join(this->_client_msgs[fd_client].erase(0, cmd[7].size()), this->_clients[fd_client], this);
			this->_client_msgs[fd_client].erase(0, this->_client_msgs[fd_client].find('\n')+1);
			break;
		case 8:
			privmsg(fd_client, this->_clients, this->_client_msgs, this->_chanels);
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

static void	send_leave(client *w_client, Chanel w_chanel){
	std::string buffer = ":" + w_client->nick + "!" + w_client->user + " PART " + w_chanel.name + "\n";
	for (std::vector<client *>::iterator it = w_chanel.user.begin(); it != w_chanel.user.end(); it++){
		send((*it)->fd, buffer.c_str(), buffer.size(), 0);
	}
	for (std::vector<client *>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		send((*it)->fd, buffer.c_str(), buffer.size(), 0);
	}
}

void Server::delChanelClient(int fd){
	for (std::vector<Chanel *>::iterator it_chanel = this->_chanels.begin(); it_chanel != this->_chanels.end(); it_chanel++){
		for (std::vector<client *>::iterator it_user = (*it_chanel)->user.begin(); it_user != (*it_chanel)->user.end(); it_user++){
			if (*it_user == this->_clients.at(fd)){
				del_user(*it_chanel, (*it_user)->nick);
				send_leave(*it_user, **it_chanel);
				break;
			}
		}
		for (std::vector<client *>::iterator it_user = (*it_chanel)->admin.begin(); it_user != (*it_chanel)->admin.end(); it_user++){
			if (*it_user == this->_clients.at(fd)){
				del_admin(*it_chanel, (*it_user)->nick);
				send_leave(*it_user, **it_chanel);
				break;
			}
		}
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

void	Server::addInvited(std::string key, std::string value) {
	this->_invited[key].push_back(value);
}

void	Server::delInvited(std::string key, std::string value) {
	std::vector<std::string> &v_inv = this->_invited[key];
	v_inv.erase(std::remove(v_inv.begin(), v_inv.end(), value), v_inv.end());
}

bool	Server::isInvited(std::string key, std::string value){
	std::vector<std::string> v_inv = this->_invited[key];
	return std::find(v_inv.begin(), v_inv.end(), value) != v_inv.end();
}
