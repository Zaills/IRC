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
	return 0;
}

static void	no_chanel(int fd, std::string arg, client *w_client) {
	std::string buffer = ": 403 " + get_only_name(w_client->nick) + " " + get_only_name(arg) + " :No such channel\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

static void	no_nick(int fd, std::string arg, client *w_client) {
	std::string buffer = ": 401 " + get_only_name(w_client->nick) + " " + get_only_name(arg) + " :No such nick/channel\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

static void not_op(int fd, std::string arg, client *w_client){
	std::string buffer = ": 482 " + get_only_name(w_client->nick) + " " + get_only_name(arg) + " :You're not channel operator\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

static void kick(std::string arg, client *w_client, Chanel w_chanel, Server *server){
	std::string buffer = ":" + w_client->user + " KICK " + w_chanel.name +  " " + get_2arg(arg, w_chanel.name) + " :" + w_client->user + "\n";
	for (std::vector<client *>::iterator it = w_chanel.user.begin(); it != w_chanel.user.end(); it++){
		send(server->get_fd((*it)->user), buffer.c_str(), buffer.size(), 0);
	}
	for (std::vector<client *>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		send(server->get_fd((*it)->user), buffer.c_str(), buffer.size(), 0);
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
		no_chanel(server->get_fd(w_client->user), arg, w_client);
		std::cout << "Chanel do not exist" << std::endl;
		return;
	}
	Chanel *w_chanel = get_w_chanel(get_only_name(arg), v_chanel);
	std::string user = get_2arg(arg, w_chanel->name);
	if (!is_admin(*w_client, *w_chanel)){
		not_op(server->get_fd(w_client->user), arg, w_client);
		std::cout << "User :" + w_client->user + " not an OP" << std::endl;
		return;
	}

	if (user.empty() || !nick_in_chanel(user, *w_chanel)){
		std::cout << "User: " + user +" not in chanel: "  + w_chanel->name << std::endl;
		no_nick(server->get_fd(w_client->user), user, w_client);
	}
	else {
		std::cout << "User: " + user +" was from: "  + w_chanel->name + " by: " + w_client->user << std::endl;
		kick(arg, w_client, *w_chanel, server);
		del_user(w_chanel, user);
		std::cout << "no !" << std::endl;
	}
}
