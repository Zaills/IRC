/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chanel.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:31:34 by marvin            #+#    #+#             */
/*   Updated: 2023/12/05 14:31:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chanel.hpp"
#include "cmd/CMD_Utils.hpp"

Chanel	creat_chanel(std::string name, std::string password, client *admin)
{
	Chanel	new_chanel;

	new_chanel.name = name;
	new_chanel.topic = "";
	new_chanel.password = password;
	new_chanel.user_limit = 0;
	new_chanel.m_i = false;
	new_chanel.m_t = true;
	new_chanel.m_o_added = false;
	new_chanel.admin.push_back(admin);
	return (new_chanel);
}

void	del_user(Chanel *chanel, std::string user) {
	for (std::vector<client *>::iterator it = chanel->user.begin(); it != chanel->user.end(); it++) {
		if (get_only_name((*it)->nick) == user){
			chanel->user.erase(it);
			return;
		}
	}
}

void	del_admin(Chanel *chanel, std::string admin) {
	for (std::vector<client *>::iterator it = chanel->admin.begin(); it != chanel->admin.end(); it++) {
		if (get_only_name((*it)->nick) == admin){
			chanel->admin.erase(it);
			return;
		}
	}
}

void	del_invited(Chanel *chanel, std::string invited) {
	for (std::vector<std::string>::iterator it = chanel->invited.begin(); it != chanel->invited.end(); it++) {
		if ((*it) == invited){
			chanel->invited.erase(it);
			return;
		}
	}
}
