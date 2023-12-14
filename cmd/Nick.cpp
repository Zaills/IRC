#include "CMD_Utils.hpp"

static void ERR_NONICKNAMEGIVEN(client *ptr)
{
	std::string buf = ": 431 " + ptr->nick + " ERR_NONICKNAMEGIVEN :No nickname given\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}

static void ERR_NICKNAMEINUSE(client *ptr)
{
	std::string buf = ": 433 " + ptr->nick + " ERR_NICKNAMEINUSE :Nickname is already in use\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}

static void ERR_ERRONEUSNICKNAME(client *ptr)
{
	std::string buf = ": 432 " + ptr->nick + " ERR_ERRONEUSNICKNAME :Erroneus nickname\n";
	send(ptr->fd, buf.c_str(), buf.size(),0);
}

static void ERR_ALREADYREGISTRED(client *ptr)
{
	std::string buf = ": 462 " + ptr->nick + " ERR_ALREADYREGISTRED :You may not reregister\n";
	send(ptr->fd, buf.c_str(), buf.size(),0);
}

void setNick(int fd,std::map<int, client *> _clients, std::map<int, std::string> _client_msgs, std::string password) //432 ERRONEUSNICKNAME ????? que faire
{
	client *ptr = _clients.at(fd);
	std::string *msgs = &(_client_msgs.at(fd));
	if (ptr->password != password)
		return ;
	else if (ptr->nick.empty() == false)
		return ERR_ALREADYREGISTRED(ptr);
	else if (check_input((*msgs), _clients) == -1)
		return ERR_NICKNAMEINUSE(ptr);
	else if (check_empty((*msgs)) == -1)
		return ERR_NONICKNAMEGIVEN(ptr);
	else
	{
		for (size_t i = 5; i < msgs->size(); i++)
		{
			if (i == 5)
			{
				if (!isalpha((*msgs)[5]))
					return ERR_ERRONEUSNICKNAME(ptr);
			}
			/* if (!isalnum((*msgs)[i]) && (*msgs)[i] != '[' && (*msgs)[i] != ']' && (*msgs)[i] != '-' &&
				(*msgs)[i] != '{' && (*msgs)[i] != '}' && (*msgs)[i] != '\\' && (*msgs)[i] != '^' && (*msgs)[i] != '`')
				return ERR_ERRONEUSNICKNAME(ptr);*/
		}
	}
	if (msgs->find('\r') == std::string::npos)
		ptr->nick = msgs->substr(5, msgs->find('\n')-5);
	else
		ptr->nick = msgs->substr(5, msgs->find('\r')-5);
	std::cout << "NICK :" << ptr->nick<<std::endl;
	LoggedIn(fd, _clients);
}
