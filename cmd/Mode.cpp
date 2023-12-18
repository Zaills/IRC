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

static void send_to_all_in_chan(Chanel *w_chanel)
{
	for (std::vector<client *>::iterator it = w_chanel->user.begin(); it != w_chanel->user.end(); it++){
		send_mode((*it), (*w_chanel));
	}
	for (std::vector<client *>::iterator it = w_chanel->admin.begin(); it != w_chanel->admin.end(); it++){
		send_mode((*it), (*w_chanel));
	}
}

static void rm_ops(Chanel *w_chanel, std::string nick)
{
	std::string buf;
	buf = ": MODE " + w_chanel->name + " -o " + nick + "\n";
	for (std::vector<client *>::iterator it = w_chanel->user.begin(); it != w_chanel->user.end(); it++){
		send((*it)->fd, buf.c_str(), buf.size(), 0);
	}
	for (std::vector<client *>::iterator it = w_chanel->admin.begin(); it != w_chanel->admin.end(); it++){
		send((*it)->fd, buf.c_str(), buf.size(), 0);
	}
}

static void mode_o(std::string type, Chanel *w_chanel, client *sender)
{
	std::string nick;
	client *ptr = NULL;
	if (type.find('\r') != std::string::npos)
		nick = type.substr(3, type.size()-5);
	else
		nick = type.substr(3, type.size()-4);
	if (nick.empty())
		return ERR_NEEDMOREPARAMS(sender);
	if (type[0] == '-')
	{
		std::vector<client *>::iterator it = w_chanel->admin.begin();
		while (it != w_chanel->admin.end())
		{
			if ((*it)->nick == nick)
				ptr = (*it);
			it++;
		}
		if (ptr == NULL)
			return ERR_NOSUCHNICK(nick, sender->fd);
		del_admin(w_chanel, nick);
		w_chanel->user.push_back(ptr);
		rm_ops(w_chanel, nick);
	}
	else{
		std::vector<client *>::iterator it = w_chanel->user.begin();
		while (it != w_chanel->user.end())
		{
			if ((*it)->nick == nick)
				ptr = (*it);
			it++;
		}
		if (ptr == NULL)
			return ERR_NOSUCHNICK(nick, sender->fd);
		w_chanel->admin.push_back(ptr);
		del_user(w_chanel, nick);
		w_chanel->m_o_added = true;
	}
}

static void	mode_k(std::string type, Chanel *w_chanel, client *ptr)
{
	if (type[0] == '-')
		w_chanel->password.clear();
	if (type.size() == 3 && type.find("\r") == std::string::npos) //nc nothinh
		return ERR_NEEDMOREPARAMS(ptr);
	if (type.size() == 4 && type.find("\r") == 2) //hexchat nothing
		return ERR_NEEDMOREPARAMS(ptr);
	if (type.find('\r') != std::string::npos){ //hexchat
		w_chanel->password = type.substr(3,type.size()-5);
	}
	else{
		w_chanel->password = type.substr(3,type.size()-4); //nc
	}
}

static void mode_l(std::string type, Chanel *w_chanel)
{
	std::string limit = &type[3];
	w_chanel->user_limit_changed = true;
	if (type[0] == '-')
		w_chanel->user_limit = 0;
	else
		w_chanel->user_limit = strtoul(limit.c_str(), NULL, 0);
	if (limit[0] == '-')
		w_chanel->user_limit = 0;
}

void	send_mode(client *w_client, Chanel w_chanel) {
	std::string buffer;
	if (w_chanel.m_t)
		buffer += "t";
	if (w_chanel.m_i)
		buffer += "i";
	if (w_chanel.m_o_added == true)
		buffer += "o " + w_chanel.admin.back()->nick + " ";
	if (w_chanel.user_limit_changed == true)
	{
		std::stringstream ss;
		ss << w_chanel.user_limit;
		buffer += "l " + ss.str() + " ";
	}
	if (!w_chanel.password.empty())
		buffer += "k " + w_chanel.password + " ";
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
			mode_k(type, w_chanel, w_client);
			send_to_all_in_chan(w_chanel);
			break;
		case 'o':
			mode_o(type, w_chanel, w_client);
			send_to_all_in_chan(w_chanel);
			w_chanel->m_o_added = false;
		break;
		case 'l':
			mode_l(type, w_chanel);
			send_to_all_in_chan(w_chanel);
			w_chanel->user_limit_changed = false;
		break;
		default:
		break;
	}
}
