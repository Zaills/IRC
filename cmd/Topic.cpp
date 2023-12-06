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

void	cmd_topic(std::string arg, client w_client, Server *server){
	(void) arg;
	(void) w_client;
	(void) server;

	std::vector<Chanel *> v_chanel = server->get_chanel();
	if (!chanel_exist(arg, v_chanel)) {
		std::cout << "No topic found" << std::cout;
	}
}
