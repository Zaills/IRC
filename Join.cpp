/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 15:57:54 by marvin            #+#    #+#             */
/*   Updated: 2023/12/05 15:57:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Join.hpp"

#include <vector>

static bool	chanel_exist(std::string chanel_name, std::vector<Chanel *> v_chanel) {
	(void) chanel_name;
	for(std::vector<Chanel *>::iterator it = v_chanel.begin(); it !=v_chanel.end() ;it++){
		if ((*it)->name == chanel_name)
			return true;
	}
	return false;
}

Chanel	&get_w_chanel(std::string chanel_name, std::vector<Chanel *> v_chanel) {
	for(std::vector<Chanel *>::iterator it = v_chanel.begin(); it != v_chanel.end(); it++){
		if ((*it)->name == chanel_name)
			return *(*it);
	}
	return *(*v_chanel.end());
}

bool	already_in_chanel(std::string name, Chanel w_chanel) {
	for(std::vector<std::string>::iterator it = w_chanel.user.begin(); it != w_chanel.user.end(); it++){
		if (*it == name)
			return true;
	}
	for(std::vector<std::string>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		if (*it == name)
			return true;
	}
	return false;
}

void	cmd_join(std::string arg, client w_client, Server *server) {
	std::vector<Chanel *> v_chanel = server->get_chanel();
	if (!chanel_exist(arg, v_chanel)) {
		Chanel *new_chanel = new Chanel(creat_chanel(arg, "", w_client.user));
		server->new_chanel(new_chanel);
		std::cout << "New chanel created: "  + new_chanel->name << std::endl;
	}
	else {
		Chanel w_chanel = get_w_chanel(arg, v_chanel);
		if (!already_in_chanel(w_client.user, w_chanel)) {
			w_chanel.user.push_back(w_client.user);
			std::cout << "user: " + w_client.user +" joined chanel: "  + w_chanel.name << std::endl;
		}
		else
			std::cout << "user: " +  w_client.user +" already in chanel: "  + w_chanel.name << std::endl;
	}
}
