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

static Chanel	&get_w_chanel(std::string chanel_name, std::vector<Chanel *> v_chanel) {
	for(std::vector<Chanel *>::iterator it = v_chanel.begin(); it != v_chanel.end(); it++){
		if ((*it)->name == chanel_name)
			return *(*it);
	}
	return *(*v_chanel.end());
}

static bool	already_in_chanel(std::string name, Chanel w_chanel) {
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

static std::string get_user(Chanel w_chanel){
	std::string out;
	for (std::vector<std::string>::iterator it = w_chanel.user.begin(); it != w_chanel.user.end(); it++){
		out += get_only_name(*it) + " ";
	}
	for (std::vector<std::string>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		out += "@" + get_only_name(*it) + " ";
	}
	return out;
}

static void	join_send_first(int fd, std::string arg, client *w_client){
	std::string buffer = ":" + get_only_name(w_client->nick)+ " JOIN " + arg + '\n';
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": MODE " + get_only_name(arg) + " +t\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": 353 " + get_only_name(w_client->nick) + " = " + arg + " :@" + get_only_name(w_client->nick) + "\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": 366 " + get_only_name(w_client->nick) + " " + arg + " :End of /NAMES list\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

static void join_send(int fd, std::string arg, client *w_client, Chanel w_chanel){
	std::string buffer = ":" + get_only_name(w_client->nick)+ " JOIN " + arg + '\n';
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": MODE " + get_only_name(arg) + " +t\n"; //need to make it good
	send(fd, buffer.c_str(), buffer.size(), 0);
	//get names list
	buffer = ": 353 " + get_only_name(w_client->nick) + " = " + arg + " :" + get_user(w_chanel) + "\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": 366 " + get_only_name(w_client->nick) + " " + arg + " :End of /NAMES list\n";
	send(fd, buffer.c_str(), buffer.size(), 0);

}

void	cmd_join(std::string arg, client * w_client, Server *server) {
	std::vector<Chanel *> v_chanel = server->get_chanel();
	if (!chanel_exist(get_only_name(arg), v_chanel)) {
		Chanel *new_chanel = new Chanel(creat_chanel(get_only_name(arg), "", w_client->user));
		server->new_chanel(new_chanel);
		std::cout << "New chanel created: "  + new_chanel->name << std::endl;
		join_send_first(server->get_fd(w_client->user), get_only_name(arg), w_client);
	}
	else {
		Chanel w_chanel = get_w_chanel(get_only_name(arg), v_chanel);
		if (!already_in_chanel(w_client->user, w_chanel)) {
			w_chanel.user.push_back(w_client->user);
			std::cout << "user: " + w_client->user +" joined chanel: "  + w_chanel.name << std::endl;
			join_send(server->get_fd(w_client->user), get_only_name(arg), w_client, w_chanel);
		}
		else{
			std::cout << "user: " +  w_client->user +" already in chanel: "  + w_chanel.name << std::endl;
			return;
		}
	}
}
