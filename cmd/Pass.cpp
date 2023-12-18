#include "CMD_Utils.hpp"

static void ERR_PASSWDMISMATCH(client *ptr)
{
	std::string buf = ": 464 " + ptr->nick + " ERR_PASSWDMISMATCH :Password incorrect\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}

void ERR_NEEDMOREPARAMS(client *ptr)
{
	std::string buf = ": 461 " + ptr->nick + " ERR_NEEDMOREPARAMS :Not enough parameters\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}

static void ERR_ALREADYREGISTRED(client *ptr)
{
	std::string buf = ": 462 " + ptr->nick + " ERR_ALREADYREGISTRED :You may not reregister\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}

void setPass(int fd, std::map<int, client *> _clients, std::map<int, std::string> _client_msgs, std::string password)
{
	client *ptr = _clients.at(fd);
	std::string *msgs = &(_client_msgs.at(fd));
	if (ptr->password.empty() == false)
		return ERR_ALREADYREGISTRED(ptr);
	if (check_empty((*msgs)) == -1)
		return ERR_NEEDMOREPARAMS(ptr);
	if (msgs->find('\r') == std::string::npos)
	{
		if (msgs->substr(5, msgs->find('\n')-5) == password)
		{
			ptr->password = msgs->substr(5, msgs->find('\n')-5);
			std::cout << "(nc)PASS :" << ptr->password << std::endl;
			LoggedIn(fd, _clients);
		}
		return ;
	}
	if (msgs->substr(5, msgs->find('\n')-6) == password)
	{
		ptr->password = msgs->substr(5, msgs->find('\n')-6);
		std::cout << "(hexchat)PASS :" << ptr->password << std::endl;
		LoggedIn(fd, _clients);
	}
	else
		return ERR_PASSWDMISMATCH(ptr);
}
