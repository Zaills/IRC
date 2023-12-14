/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 13:40:03 by marvin            #+#    #+#             */
/*   Updated: 2023/12/14 13:40:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CMD_Utils.hpp"

// static void	mode_t(std::string type, Chanel *w_chanel){

// }


static void	mode_k(std::string type, Chanel *w_chanel)
{ // need to protect with JOIN to not be able to join a channel with pass without pass
	std::cout << "IN MODE PASSWORD\n";
	if (type[0] == '-')
		w_chanel->password.clear();
	else if (type.find('\r') != std::string::npos){ //hexchat
		w_chanel->password = type.substr(3,type.size()-5);
		std::cout << "PASS ADDDED:" << w_chanel->password;
	}
	else{
		w_chanel->password = type.substr(3,type.size()-4); //nc
		std::cout << "PASS ADDDED:" << w_chanel->password;
	}
}

static void mode_o(std::string type, Chanel *w_chanel, client *ptr)
{
	std::cout << "IN MODE OPERATOR\n";
	bool found = false;
	std::string nick;
	client *ptr = NULL;
	std::vector<client *>::iterator it = w_chanel->user.begin();
	std::vector<client *>::iterator it_admin = w_chanel->admin.begin();
	while (it_admin != w_chanel->admin.end())
	{
		if ((*it_admin)->nick == ptr->nick)
			found = true;
		it_admin++;
	}
	if (found == false)// une personne non admin ne peux pas executer cette cmd
		return ;
	if (type.find('\r') != std::string::npos)
		nick = type.substr(3, type.size()-5);
	else
		nick = type.substr(3, type.size()-4);
	if (nick.empty()) // pas de nom donner
	{
		std::cout << "error\n";
		return ;
	}
	while (it != w_chanel->user.end())
	{
		if ((*it)->nick == nick)
			ptr = (*it);
		it++;
	}
	std::cout << "NICK :" << nick << "\n";
	if (ptr == NULL) // le user existe pas dans le canal
		return;
	if (type[0] == '-')
		del_admin(w_chanel, nick);
	else{
		w_chanel->admin.push_back(ptr);
		w_chanel->user.erase(it-1);
	}
}

static void mode_l(std::string type, Chanel *w_chanel)
{
	std::string limit = &type[3];
	std::cout << "IN USER LIMIT MODE\n";
	if (type[0] == '-')
		w_chanel->user_limit = 0;
	else
		w_chanel->user_limit = strtoul(limit.c_str(), NULL, 0);
	std::cout << "USER LIMIT :" << w_chanel->user_limit;
}


void	send_mode(client *w_client, Chanel w_chanel) {
	std::string buffer;
	if (w_chanel.m_t)
		buffer += "t";
	if (w_chanel.m_i)
		buffer += "i";
	//add the check

	if (!w_chanel.password.empty())
		buffer += "k " + w_chanel.password;
	if (w_chanel.m_o_added == true)
	{
		w_chanel.m_o_added = false;
		buffer += "o";
	}
	if (w_chanel.user_limit != 0)
		buffer += "l";

	buffer += "\n";
	buffer = ": MODE " + w_chanel.name + " +" + buffer;
	send(w_client->fd, buffer.c_str(), buffer.size(), 0);
}

void	cmd_mode(std::string arg, client *w_client, Server *server) {
	std::vector<Chanel *> *v_chanel = server->get_chanel();
	if (!chanel_exist(get_only_name(arg), *v_chanel)) {
		no_chanel(w_client->fd, arg, w_client);
		std::cout << "Chanel: " + get_only_name(arg) + " do not exist" << std::endl;
		return;
	}
	Chanel *w_chanel = get_w_chanel(get_only_name(arg), v_chanel);
	if (!already_in_chanel(w_client->user, *w_chanel)) {
		not_on_chanel(w_client->fd, arg, w_client);
		std::cout << "User: " + w_client->nick + " is not in Chanel: " + w_chanel->name << std::endl;
		return;
	}
	std::string type = get_2arg(arg, w_chanel->name);
	if (get_only_name(type).empty() || get_only_name(type).size() < 1) {
		send_mode(w_client, *w_chanel);
		return;
	}
	if (!is_admin(*w_client, *w_chanel)){
		not_op(w_client->fd, arg, w_client);
		std::cout << "User: " + w_client->nick + " is not Op in Chanel: " + w_chanel->name << std::endl;
		return;
	}
	switch (type[1]) {
		case 'i':
			if (type[0] == '+')
				w_chanel->m_i = 1;
			if (type[0] == '-')
				w_chanel->m_t = 0;
		break;
		case 't':
			if (type[0] == '+')
				w_chanel->m_t = 1;
			if (type[0] == '-')
				w_chanel->m_t = 0;
		break;
		case 'k':
			mode_k(type, w_chanel);
			send_mode(w_client, (*w_chanel));
			break;
		case 'o':
			mode_o(type, w_chanel, w_client);
			send_mode(w_client, (*w_chanel));
		break;
		case 'l':
			mode_l(type, w_chanel);
			send_mode(w_client, (*w_chanel));
		break;
		default:
		break;
	}
}
