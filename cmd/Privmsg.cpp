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

static void send_to_channel(int fd, std::vector<Chanel *> _chanels, std::string text, std::string recv, std::string sender)
{
	std::vector<Chanel *>::const_iterator it_chan = _chanels.begin();
	std::vector<client *>::const_iterator it_user;
	std::vector<client *>::const_iterator it_admin;
	std::string buf;
	Chanel *ptr = NULL;
	while (it_chan != _chanels.end())
	{
		if ((*it_chan)->name == recv)
		{
			ptr = (*it_chan);
			break ;
		}
		it_chan++;
	}
	it_user = ptr->user.begin();
	it_admin = ptr->admin.begin();
	while (it_user != ptr->user.end())
	{
		if ((*it_user)->fd != fd)
		{
			buf = ":"+ sender + " PRIVMSG " + ptr->name + " :" + text;
			send((*it_user)->fd,buf.c_str(), buf.size(),0);
		}
		it_user++;
	}
	while (it_admin != ptr->admin.end())
	{
		if ((*it_admin)->fd != fd)
		{
			buf = ":" + sender + " PRIVMSG " + ptr->name + " :" + text;
			send((*it_admin)->fd,buf.c_str(), buf.size(),0);
		}
		it_admin++;
	}
}

void privmsg(int fd, std::map<int, client *> _clients, std::map<int, std::string> _client_msgs, std::vector<Chanel *> _chanels)
{
	std::string msgs = &(_client_msgs.at(fd)[8]);
	std::string receiver = msgs.substr(0,msgs.find(' '));
	std::string text;
	std::map<int, client*>::const_iterator it = _clients.begin();
	std::vector<Chanel *>::const_iterator it_chan = _chanels.begin();
	bool user = false, chanel = false;
	//NEED ERR_CANNOTSENDTOCHAN (le mode doit etre fait avant)
	if (receiver.find("\n") != std::string::npos)
		receiver.erase(receiver.size()-1,receiver.size());
	if (receiver[0] == '\r' && receiver.size() == 1)
		return ERR_NORECIPIENT(_clients.at(fd), "PRIVMSG");
	text = &msgs[receiver.size()+1];
	if (text.empty())
		return ERR_NOTEXTTOSEND(_clients.at(fd));
	if (text[0] == ':') {
		text.erase(0, 1);
	}
	while (it != _clients.end()){
		if ((*it).second->nick == receiver)
		{
			user = true;
			break ;
		}
		it++;
	}
	while (it_chan != _chanels.end()){
		if ((*it_chan)->name == receiver)
		{
			chanel = true;
			break ;
		}
		it_chan++;
	}
	if (user == false && chanel == false)
		return ERR_NOSUCHNICK(receiver, fd);
	if (chanel == true)
	{
		send_to_channel(fd, _chanels, text, receiver, _clients.at(fd)->nick + "!" + _clients.at(fd)->user);
		return ;
	}
	std::string buf =":" + _clients.at(fd)->nick + "!" + _clients.at(fd)->user + " PRIVMSG " + receiver + " :" + text;
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




