/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 13:56:03 by marvin            #+#    #+#             */
/*   Updated: 2023/12/06 13:56:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CMD_Utils.hpp"
#include <string>

static std::string get_2arg(std::string arg, std::string ch_name) {
	arg.erase(0, ch_name.size()+ 1);
	while(arg[0] == ' ')
		arg.erase(0, arg.find(' '));
	if (arg[0] != '\n' || arg[0] != '\r')
		return get_only_name(arg);
	return arg.erase(0, arg.size());
}

static void	send_topic(std::string arg, client *w_client, Chanel *w_chanel) {
	if (w_chanel->topic.empty()){
		std::string buffer = ": 331 " + get_only_name(w_client->nick) + " " + get_only_name(arg) + " :No topic is set\n";
		send(w_client->fd, buffer.c_str(), buffer.size(), 0);
	}
	else {
		std::string buffer = ": 332" + get_only_name(w_client->nick) + " " + get_only_name(arg) + " :" + get_only_name(w_chanel->topic) + "\n";
		send(w_client->fd, buffer.c_str(), buffer.size(), 0);
	}
}

static void	set_topic(std::string arg, client *w_client, Chanel *w_chanel) {
	if (w_chanel)
}

void	cmd_topic(std::string arg, client *w_client, Server *server) {
	std::vector<Chanel *> *v_chanel = server->get_chanel();
	if (!chanel_exist(arg, *v_chanel)) {
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
	std::string topic = get_2arg(arg, w_chanel->name);
	if (topic.empty())
		send_topic(arg, w_client, w_chanel);
	else {
	//settopic
	}
}
