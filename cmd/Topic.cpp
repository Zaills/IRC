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

void	send_topic(client *w_client, Chanel w_chanel) {
	if (w_chanel.topic.empty()){
		std::string buffer = ": 331 " + get_only_name(w_client->nick) + " " + get_only_name(w_chanel.name) + " :No topic is set\n";
		send(w_client->fd, buffer.c_str(), buffer.size(), 0);
	}
	else {
		std::string buffer = ": 332 " + get_only_name(w_client->nick) + " " + get_only_name(w_chanel.name) + " :" + w_chanel.topic + "\n";
		send(w_client->fd, buffer.c_str(), buffer.size(), 0);
	}
}

static	void	send_all(Chanel w_chanel){
	for (std::vector<client *>::iterator it = w_chanel.user.begin(); it != w_chanel.user.end(); it++){
		send_topic(*it, w_chanel);
	}
	for (std::vector<client *>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		send_topic(*it, w_chanel);
	}
}

static	void	set_topic(std::string arg, client *w_client, Chanel *w_chanel) {
	if (w_chanel->m_t && !is_admin(*w_client, *w_chanel)){
		std::string buffer = ": 482 " + get_only_name(w_client->nick) + " " + get_only_name(w_chanel->name) + " :You're not channel operator\n";
		send(w_client->fd, buffer.c_str(), buffer.size(), 0);
	}
	else{
		w_chanel->topic = get_2arg(arg, w_chanel->name);
		if (w_chanel->topic[0] == ':')
			w_chanel->topic.erase(0, 1);
		send_all(*w_chanel);
	}
}

void	cmd_topic(std::string arg, client *w_client, Server *server) {
	std::vector<Chanel *> *v_chanel = server->get_chanel();
	if (!chanel_exist(get_only_name(arg), *v_chanel)) {
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
		send_topic(w_client, *w_chanel);
	else
		set_topic(arg, w_client, w_chanel);
}
