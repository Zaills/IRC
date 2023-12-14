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

bool	chanel_exist(std::string chanel_name, std::vector<Chanel *> v_chanel) {
	(void) chanel_name;
	for(std::vector<Chanel *>::iterator it = v_chanel.begin(); it !=v_chanel.end() ;it++){
		if ((*it)->name == chanel_name)
			return true;
	}
	return false;
}

Chanel	*get_w_chanel(std::string chanel_name, std::vector<Chanel *> *v_chanel) {
	for(std::vector<Chanel *>::iterator it = v_chanel->begin(); it != v_chanel->end(); it++){
		if ((*it)->name == chanel_name)
			return *it;
	}
	return *v_chanel->end();
}

bool	already_in_chanel(std::string name, Chanel w_chanel) {
	for(std::vector<client *>::iterator it = w_chanel.user.begin(); it != w_chanel.user.end(); it++){
		if ((*it)->user == name)
			return true;
	}
	for(std::vector<client *>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		if ((*it)->user == name)
			return true;
	}
	return false;
}

std::string get_only_name(std::string arg) {
	std::string out = arg.substr(0, arg.find(' '));

	if (out.size() > arg.substr(0, arg.find('\n')).size())
		out = arg.substr(0, arg.find('\n'));
	if (out.size() > arg.substr(0, arg.find('\r')).size())
		out = arg.substr(0, arg.find('\r'));
	return out;
}

bool	is_admin(client user, Chanel w_chanel) {

	for(std::vector<client *>::iterator it = w_chanel.admin.begin(); it != w_chanel.admin.end(); it++){
		if ((*it)->user == user.user)
			return true;
	}
	return false;
}

int check_input(std::string msgs, std::map<int, client *> _clients)
{
	if (msgs[0] == '\n' && msgs.size() == 1)
		return -1;
	std::string str = &msgs[5];
	if (str.find('\r') == std::string::npos)
		str = str.substr(0,msgs.find('\n')-5);
	else
		str = str.substr(0,msgs.find('\r')-5);
	std::map<int, client*>::const_iterator it = _clients.begin();
	while (it != _clients.end()){
		if ((*it).second->nick == str)
			return -1;
		it++;
	}
	return 1;
}

int check_empty(std::string msgs)
{
	std::string temp = &msgs[3];
	if (temp.size() == 1)
		return -1;
	temp = &msgs[4];
	for(size_t i = 0; i < temp.size(); i++)
		if (!isspace(temp[i]))
			return 1;
	return -1;
}

void LoggedIn(int fd, std::map<int, client *> _clients) {
	client *ptr = _clients.at(fd);
	if (!ptr->nick.empty() && !ptr->user.empty())
	{
		ptr->is_logged = true;
		std::cout << "A CLIENT HAS BEEN CONNECTED !\n";
	}
}

  //Send
void	no_chanel(int fd, std::string arg, client *w_client) {
	std::string buffer = ": 403 " + get_only_name(w_client->nick) + " " + get_only_name(arg) + " :No such channel\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

void	no_nick(int fd, std::string arg, client *w_client) {
	std::string buffer = ": 401 " + get_only_name(w_client->nick) + " " + get_only_name(arg) + " :No such nick/channel\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

void	not_op(int fd, std::string arg, client *w_client){
	std::string buffer = ": 482 " + get_only_name(w_client->nick) + " " + get_only_name(arg) + " :You're not channel operator\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}

void	not_on_chanel(int fd, std::string arg, client *w_client){
	std::string buffer = ": 442 " + get_only_name(w_client->nick) + " " + get_only_name(arg) + " :You're not on that channel\n";
	send(fd, buffer.c_str(), buffer.size(), 0);
}
