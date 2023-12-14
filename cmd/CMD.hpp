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

#endif
