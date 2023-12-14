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

void	cmd_mode(std::string arg, client *w_client, Server *Server){
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
	if (!is_admin(*w_client, *w_chanel)){
		not_op(w_client->fd, arg, w_client);
		std::cout << "User: " + w_client->nick + " is not Op in Chanel: " + get_only_name(arg) << std::endl;
		return;
	}
	std::string type = get_2arg(arg, w_chanel->name);
	if (type.empty() || type.size() < 1) {
		std::string buffe = ": 461 " + w_client->nick + " ERR_NEEDMOREPARAMS :Not enough parameters\n";
		send(w_client->fd, buffe.c_str(), buffe.size(), 0);
		return;
	}
	switch (type[1]) {
		case 'i':
		break;
		case 't':
		break;
		case 'k':
		break;
		case 'o':
		break;
		case 'l':
		break;
	}
}
