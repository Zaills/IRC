/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:01:33 by marvin            #+#    #+#             */
/*   Updated: 2023/12/06 14:01:33 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
# define CMD_HPP

# include "../Server.hpp"

void	cmd_topic(std::string arg, client *w_client, Server *server);
void	cmd_join(std::string arg, client *w_client, Server *server);
void	cmd_kick(std::string arg, client *w_client, Server *Server);
void	cmd_mode(std::string arg, client *w_client, Server *server);
void	privmsg(int fd, std::map<int, client *> _clients, std::map<int, std::string> _client_msgs, std::vector<Chanel *> _chanels);
void	setUser(int fd, std::map<int, client *> _clients, std::map<int, std::string> _client_msgs, std::string password);
void	setPass(int fd, std::map<int, client *> _clients, std::map<int, std::string> _client_msgs, std::string password);
void	setNick(int fd, std::map<int, client *> _clients, std::map<int, std::string> _client_msgs, std::string password);

#endif
