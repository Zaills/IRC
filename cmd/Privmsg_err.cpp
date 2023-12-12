#include "Privmsg_err.hpp"

/*
411 ERR_NORECIPIENT
":No recipient given (<commande>)"
*/
void ERR_NORECIPIENT(client *ptr, std::string cmd_name)
{
	std::string buf =": 411 " + ptr->nick + " ERR_NORECIPIENT :No recipient given (" + cmd_name + ")\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}
/*
401 ERR_NOSUCHNICK
"<pseudonyme> :No such nick/channel"
*/
void ERR_NOSUCHNICK(client *ptr)
{
	std::string buf =": 401 ERR_NOSUCHNICK " + ptr->nick + " :No such nick/channel\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}
/*
412 ERR_NOTEXTTOSEND
":No text to send"
 */
void ERR_NOTEXTTOSEND(client *ptr)
{
	std::string buf =": 412 " + ptr->nick + " ERR_NORECIPIENT :No text to send\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
}
/*
404 ERR_CANNOTSENDTOCHAN
"<nom de canal> :Cannot send to channel
 */
void ERR_CANNOTSENDTOCHAN(client *ptr, std::string chan_name)
{
	std::string buf =": 404 ERR_CANNOTSENDTOCHAN" + chan_name + " :Cannot send to channel\n";
	send(ptr->fd, buf.c_str(), buf.size(), 0);
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




