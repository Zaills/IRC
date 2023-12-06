/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CMD_Utils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:05:07 by marvin            #+#    #+#             */
/*   Updated: 2023/12/06 14:05:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_UTILS_CPP
#define CMD_UTILS_CPP

# include "../Server.hpp"

bool	chanel_exist(std::string chanel_name, std::vector<Chanel *> v_chanel);
std::string get_only_name(std::string arg);

#endif
