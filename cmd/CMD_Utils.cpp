/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD_Utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:20:33 by marvin            #+#    #+#             */
/*   Updated: 2023/12/06 14:20:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CMD_Utils.hpp"

bool	chanel_exist(std::string chanel_name, std::vector<Chanel *> v_chanel){
	(void) chanel_name;
	for(std::vector<Chanel *>::iterator it = v_chanel.begin(); it !=v_chanel.end() ;it++){
		if ((*it)->name == chanel_name)
			return true;
	}
	return false;
}

std::string get_only_name(std::string arg){
	std::string out = arg.substr(0, arg.find(' '));

	if (out.size() > arg.substr(0, arg.find('\n')).size())
		out = arg.substr(0, arg.find('\n'));
	if (out.size() > arg.substr(0, arg.find('\r')).size())
		out = arg.substr(0, arg.find('\r'));
	return out;
}
