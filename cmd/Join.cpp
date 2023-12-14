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

# include "CMD_Utils.hpp"

static std::string get_user(Chanel w_chanel, client w_client){
	std::string out;
	std::string buffer;
	for (std::vector<client *>::iterator it = w_chanel.user.begin(); it != w_chanel.user.end(); it++){
		out += get_only_name((*it)->nick) + " ";
		buffer = ":" + w_client.nick + "!" + w_client.user + " JOIN :" + w_chanel.name + "\n";
		send((*it)->fd, buffer.c_str(), buffer.size(), 0);
	}
	for (std::vector<client *>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		out += "@" + get_only_name((*it)->nick) + " ";
		buffer = ":" + w_client.nick + "!" + w_client.user + " JOIN :" + w_chanel.name + "\n";
		send((*it)->fd, buffer.c_str(), buffer.size(), 0);
	}
	return out;
}

static void	join_send_first(int fd, std::string arg, client *w_client){
	std::string buffer = ":" + get_only_name(w_client->nick) + " JOIN " + arg + '\n';
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": MODE " + get_only_name(arg) + " +t\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": 353 " + get_only_name(w_client->nick) + " = " + arg + " :@" + get_only_name(w_client->nick) + "\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": 366 " + get_only_name(w_client->nick) + " " + arg + " :End of /NAMES list\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

static void join_send(int fd, std::string arg, client *w_client, Chanel w_chanel){
	//get names list
	std::string buffer = ": 353 " + get_only_name(w_client->nick) + " = " + arg + " :" + get_user(w_chanel, *w_client) + "\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": 366 " + get_only_name(w_client->nick) + " " + arg + " :End of /NAMES list\n";
	send(fd, buffer.c_str(), buffer.size(), 0);

	buffer = ": MODE " + get_only_name(arg) + " +t\n"; //need to make it good
	send(fd, buffer.c_str(), buffer.size(), 0);
}

static void bad_name(int fd, std::string arg, client *w_client){
	std::string buffer = ": 476 " + get_only_name(w_client->nick) + " " + arg + " :Invalid channel name\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

void	cmd_join(std::string arg, client *w_client, Server *server) {
	std::vector<Chanel *> *v_chanel = server->get_chanel();
	if (arg[0] != '#' && arg[0] != '&'){
		bad_name(w_client->fd, get_only_name(arg), w_client);
		std::cout << "Bad chanel name: " + get_only_name(arg) << std::endl;
		return;
	}
	if (!chanel_exist(get_only_name(arg), *v_chanel)) {
		Chanel *new_chanel = new Chanel(creat_chanel(get_only_name(arg), "", w_client));
		server->new_chanel(new_chanel);
		std::cout << "New chanel created: "  + new_chanel->name << std::endl;
		join_send_first(w_client->fd, get_only_name(arg), w_client);
	}
	else {
		Chanel *w_chanel = get_w_chanel(get_only_name(arg), v_chanel);
		if (!already_in_chanel(w_client->user, *w_chanel)) {
			w_chanel->user.push_back(w_client);
			std::cout << "user: " + w_client->user +" joined chanel: "  + w_chanel->name << std::endl;
			join_send(w_client->fd, get_only_name(arg), w_client, *w_chanel);
		}
		else{
			std::cout << "user: " +  w_client->user +" already in chanel: "  + w_chanel->name << std::endl;
			return;
		}
	}
}
