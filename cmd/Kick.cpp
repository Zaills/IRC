/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Leave.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 13:19:29 by marvin            #+#    #+#             */
/*   Updated: 2023/12/11 13:19:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CMD_Utils.hpp"

static std::string get_2arg(std::string arg, std::string ch_name){
	arg.erase(0, ch_name.size()+ 1);
	while(arg[0] == ' ')
		arg.erase(0, arg.find(' '));
	if (arg[0] != '\n' || arg[0] != '\r')
		return get_only_name(arg);
	return arg.erase(0, arg.size());
}

static std::string get_reson(std::string arg, std::string ch_name){
	std::string arg2 = get_2arg(arg, ch_name);
	arg.erase(0, ch_name.size()+ 1);
	while(arg[0] == ' ')
		arg.erase(0, arg.find(' '));
	arg.erase(0, arg2.size()+ 1);
	while(arg[0] == ' ')
		arg.erase(0, arg.find(' '));
	if (arg[0] == '\n' || arg[0] == '\r')
		return arg.erase(0, arg.size());
	if (arg[0] == ':')
		return arg.erase(0, 1);
	return arg;
}

static void kick(std::string arg, client *w_client, Chanel w_chanel){
	std::string reson = get_reson(arg, w_chanel.name);
	std::string buffer;
	if (reson.empty())
		buffer = ":" + get_only_name(w_client->user) + " KICK " + w_chanel.name +  " " + get_2arg(arg, w_chanel.name) + " :No reason given\n";
	else
		buffer = ":" + get_only_name(w_client->user) + " KICK " + w_chanel.name +  " " + get_2arg(arg, w_chanel.name) + " :" + reson;
	for (std::vector<client *>::iterator it = w_chanel.user.begin(); it != w_chanel.user.end(); it++){
		send((*it)->fd, buffer.c_str(), buffer.size(), 0);
	}
	for (std::vector<client *>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		send((*it)->fd, buffer.c_str(), buffer.size(), 0);
	}
}

static bool	nick_in_chanel(std::string name, Chanel w_chanel) {
	for(std::vector<client *>::iterator it = w_chanel.user.begin(); it != w_chanel.user.end(); it++){
		if (get_only_name((*it)->nick) == name)
			return true;
	}
	for(std::vector<client *>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		if (get_only_name((*it)->nick) == name)
			return true;
	}
	return false;
}

void	cmd_kick(std::string arg, client *w_client, Server *server){
	std::vector<Chanel *> *v_chanel = server->get_chanel();


	if (!chanel_exist(get_only_name(arg), *v_chanel)){
		no_chanel(w_client->fd, arg, w_client);
		std::cout << "Chanel: " + get_only_name(arg) + " do not exist" << std::endl;
		return;
	}
	Chanel *w_chanel = get_w_chanel(get_only_name(arg), v_chanel);
	if (!already_in_chanel(w_client->user, *w_chanel)) {
		not_on_chanel(w_client->fd, arg, w_client);
		std::cout << "User: " + w_client->nick + " is not in Chanel: " + get_only_name(arg) << std::endl;
		return;
	}
	if (!is_admin(*w_client, *w_chanel)){
		not_op(w_client->fd, arg, w_client);
		std::cout << "User :" + w_client->user + " not an OP" << std::endl;
		return;
	}
	std::string user = get_2arg(arg, w_chanel->name);

	if (user.empty() || !nick_in_chanel(user, *w_chanel)){
		std::cout << "User: " + user +" not in chanel: "  + w_chanel->name << std::endl;
		no_nick(w_client->fd, user, w_client);
	}
	else {
		std::cout << "User: " + user +" was from: "  + w_chanel->name + " by: " + w_client->user << std::endl;
		kick(arg, w_client, *w_chanel);
		del_user(w_chanel, user);
	}
}
