/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 15:16:42 by marvin            #+#    #+#             */
/*   Updated: 2023/12/14 15:16:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "CMD_Utils.hpp"

void cmd_invite(std::string arg, client *w_client, Server *server) {
	std::vector<Chanel *> *v_chanel = server->get_chanel();
	std::string ch_name = get_only_name(get_2arg(arg, get_only_name(arg)));
	if (!chanel_exist(ch_name, *v_chanel)) {
		server->addInvited(ch_name, get_only_name(arg));
		std::cout << "User: " + get_only_name(arg) + " Invited to Chanel: " + get_2arg(arg, get_only_name(arg)) << std::endl;
		return;
	}
	Chanel *w_chanel = get_w_chanel(ch_name, v_chanel);
	if (!already_in_chanel(w_client->user, *w_chanel)) {
		not_on_chanel(w_client->fd, arg, w_client);
		std::cout << "User: " + w_client->nick + " is not in Chanel: " + get_only_name(arg) << std::endl;
		return;
	}
	if (!is_admin(*w_client, *w_chanel)){
		not_op(w_client->fd, arg, w_client);
		std::cout << "User: " + w_client->nick + " is not Op in Chanel: " + w_chanel->name << std::endl;
		return;
	}
	std::string invited = get_only_name(arg);
	server->addInvited(ch_name, get_only_name(arg));
	std::cout << "User: " + get_only_name(arg) + " Invited to Chanel: " + get_2arg(arg, get_only_name(arg)) << std::endl;

}
