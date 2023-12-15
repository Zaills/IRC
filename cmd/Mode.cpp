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

void	send_mode(client *w_client, Chanel w_chanel) {
	std::string buffer;
	if (w_chanel.m_t)
		buffer += "t";
	if (w_chanel.m_i)
		buffer += "i";
	if (w_chanel.user_limit)
		buffer += "l";
	//add the check

	if (buffer.empty())
		return;
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
				w_chanel->m_i = 0;
		break;
		case 't':
			if (type[0] == '+')
				w_chanel->m_t = 1;
			if (type[0] == '-')
				w_chanel->m_t = 0;
		break;
		case 'k':
		break;
		case 'o':
		break;
		case 'l':
		break;
		default:
		break;
	}
}
