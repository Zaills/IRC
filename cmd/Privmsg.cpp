#include "CMD_Utils.hpp"

static void ERR_NORECIPIENT(client *ptr, std::string cmd_name)
{
	std::string buf =": 411 ERR_NORECIPIENT :No recipient given (" + cmd_name + ")\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}

static void ERR_NOSUCHNICK(std::string name, int fd)
{
	std::string buf =": 401 ERR_NOSUCHNICK " + name + " :No such nick/channel\n";
	send(fd, buf.c_str(), buf.size(), 0);
}

static void ERR_NOTEXTTOSEND(client *ptr)
{
	std::string buf =": 412 " + ptr->nick + " :No text to send\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}

/* static void ERR_CANNOTSENDTOCHAN(client *ptr, std::string chan_name)
{
	std::string buf =": 404 ERR_CANNOTSENDTOCHAN" + chan_name + " :Cannot send to channel\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
} */

void privmsg(int fd, std::map<int, client *> _clients, std::map<int, std::string> _client_msgs)
{
	std::string msgs = &(_client_msgs.at(fd)[8]);
	std::string receiver = msgs.substr(0,msgs.find(' '));
	std::string text = &msgs[receiver.size()+1];
	std::map<int, client*>::const_iterator it = _clients.begin();
	bool found = false;
	//NEED ERR_CANNOTSENDTOCHAN (le mode doit etre fait avant)
	if (receiver.find("\n") != std::string::npos)
		receiver.erase(receiver.size()-1,receiver.size());
	if (receiver[0] == '\r' && receiver.size() == 1)
		return ERR_NORECIPIENT(_clients.at(fd), "PRIVMSG");
	if (text.empty())
		return ERR_NOTEXTTOSEND(_clients.at(fd));
	while (it != _clients.end() && found == false){
		if ((*it).second->nick == receiver)
		{
			found = true;
			break ;
		}
		it++;
	}
	if (found == false)
		return ERR_NOSUCHNICK(receiver, fd);
	std::string buf =":" + _clients.at(fd)->nick + " PRIVMSG " + receiver + " :" + text;
	send((*it).second->fd, buf.c_str(), buf.size(),0);
}

//		c est pour $privmsg et #privmsg peut etre pas a gerer

/*
414 ERR_WILDTOPLEVEL
"<masque> :Wildcard in toplevel domain"
 */
//
/*
413 ERR_NOTOPLEVEL
"<masque> :No toplevel domain specified"
 */


//		utilisateur@hôte pour lequel utilisateur@hôte a plusieurs occurrences (peut etre pas a gerer egalement)

/* 407 ERR_TOOMANYTARGETS
"<destination> :Duplicate recipients. No message delivered"
 */




