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

#include "CMD.hpp"
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

static void	join_send_first(int fd, client *w_client, Chanel w_chanel){
	std::string buffer = ":" + get_only_name(w_client->nick) + "!" + w_client->user + " JOIN " + w_chanel.name + '\n';
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": 353 " + get_only_name(w_client->nick) + " = " + w_chanel.name + " :@" + get_only_name(w_client->nick) + "\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": 366 " + get_only_name(w_client->nick) + " " + w_chanel.name + " :End of /NAMES list\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

static void join_send(int fd, client *w_client, Chanel w_chanel){
	//get names list
	std::string buffer = ": 353 " + get_only_name(w_client->nick) + " = " + w_chanel.name + " :" + get_user(w_chanel, *w_client) + "\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
	buffer = ": 366 " + get_only_name(w_client->nick) + " " + w_chanel.name + " :End of /NAMES list\n";
	send(fd, buffer.c_str(), buffer.size(), 0);

	send_topic(w_client, w_chanel);
}

static void bad_name(int fd, std::string arg, client *w_client){
	std::string buffer = ": 476 " + get_only_name(w_client->nick) + " " + arg + " :Invalid channel name\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

static void	not_invited(client *w_client, Chanel w_chanel) {
	std::string buffer = ": 473 " + get_only_name(w_client->nick) + " " + w_chanel.name + " :Cannot join channel (+i)\n";
	send(w_client->fd, buffer.c_str(), buffer.size(), 0);
}

static std::string get_pass(std::string start)
{
	std::string pass;
	pass = &start[get_only_name(start).size() + 1];
	if (pass.find('\r') == std::string::npos)
		pass = pass.substr(0, pass.find('\n'));
	else
		pass =  pass.substr(0, pass.find('\n')-1);
	return pass;
}

static void ERR_BADCHANNELKEY(Chanel *chanel, client *ptr)
{
	std::string buffer = ": 475 " + get_only_name(ptr->nick) + " " + chanel->name + " :Cannot join channel (+k)\n";
	send(ptr->fd, buffer.c_str(), buffer.size(), 0);
}

static void ERR_CHANNELISFULL(Chanel *chanel, client *ptr)
{
	std::string buffer = ": 471 " + get_only_name(ptr->nick) + " " + chanel->name + " :Cannot join channel (+l)\n";
	send(ptr->fd, buffer.c_str(), buffer.size(), 0);
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
		join_send_first(w_client->fd, w_client, *new_chanel);
	}
	else {
		Chanel *w_chanel = get_w_chanel(get_only_name(arg), v_chanel);
		if (already_in_chanel(w_client->user, *w_chanel)) {
			std::cout << "user: " +  w_client->user +" already in chanel: "  + w_chanel->name << std::endl;
			return;
		}
		else if (w_chanel->m_i && !server->isInvited(w_chanel->name, w_client->nick)) {
			not_invited(w_client, *w_chanel);
			std::cout << "user: " +  w_client->user +" not invited in Chanel: "  + w_chanel->name << std::endl;
			return;
		}
		else if (w_chanel->password == get_pass(arg) && (w_chanel->user_limit == 0 || w_chanel->user.size() + w_chanel->admin.size() < w_chanel->user_limit)){
			if (w_chanel->m_i && server->isInvited(w_chanel->name, w_client->nick))
				server->delInvited(w_chanel->name, w_client->nick);
			std::cout << w_chanel->user_limit << std::endl;
			std::cout << w_chanel->user.size() + w_chanel->admin.size() << std::endl;
			w_chanel->user.push_back(w_client);
			std::cout << "user: " + w_client->user +" joined chanel: "  + w_chanel->name << std::endl;
			join_send(w_client->fd, w_client, *w_chanel);
			return;
		}
		if (w_chanel->password != get_pass(arg))
			return ERR_BADCHANNELKEY(w_chanel, w_client);
		if (w_chanel->user_limit && w_chanel->user.size() + w_chanel->admin.size() >= w_chanel->user_limit)
			return ERR_CHANNELISFULL(w_chanel, w_client);
	}
}
