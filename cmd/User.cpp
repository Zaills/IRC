#include "CMD_Utils.hpp"

static void ERR_ALREADYREGISTRED(client *ptr)
{
	std::string buf = ": 462 " + ptr->nick + " ERR_ALREADYREGISTRED :You may not reregister\n";
	send(ptr->fd, buf.c_str(), buf.size(),0);
}

static void ERR_NEEDMOREPARAMS(client *ptr)
{
	std::string buf = ": 461 " + ptr->nick + " ERR_NEEDMOREPARAMS :Not enough parameters\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}

void setUser(int fd, std::map<int, client *> _clients, std::map<int, std::string> _client_msgs, std::string password)
{
	client *ptr = _clients.at(fd);
	std::string *msgs = &(_client_msgs.at(fd));
	if (ptr->password != password)
		return ;
	if (ptr->user.empty() == false)
		return ERR_ALREADYREGISTRED(ptr);
	else if (check_empty((*msgs)) == -1)
		return ERR_NEEDMOREPARAMS(ptr);
	if (msgs->find('\r') == std::string::npos)
		ptr->user = msgs->substr(5, msgs->find('\n')-5);
	else
		ptr->user = msgs->substr(5, msgs->find("*")-8);
	std::cout << "USER :" << ptr->user << std::endl;
	LoggedIn(fd, _clients);
}
