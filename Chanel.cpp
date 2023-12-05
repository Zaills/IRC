/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chanel.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:31:34 by marvin            #+#    #+#             */
/*   Updated: 2023/12/05 14:31:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chanel.hpp"

Chanel	creat_chanel(std::string name, std::string password, std::string admin)
{
	Chanel	new_chanel;

	new_chanel.name = name;
	new_chanel.password = password;
	new_chanel.user_limit = 0;
	new_chanel.invite = false;
	new_chanel.admin.push_back(admin);
	return (new_chanel);
}

void	del_user(Chanel *chanel, std::string user)
{
	for (unsigned int i = 0; i < chanel->user.size(); ++i)
	{
		if (chanel->user[i] == user)
		{
			chanel->user.erase(chanel->user.begin() + i);
			return ;
		}
	}
}

void	del_admin(Chanel *chanel, std::string admin)
{
	for (unsigned int i = 0; i < chanel->admin.size(); ++i)
	{
		if (chanel->admin[i] == admin)
		{
			chanel->admin.erase(chanel->admin.begin() + i);
			return ;
		}
	}
}

void	del_invited(Chanel *chanel, std::string invited)
{
	for (unsigned int i = 0; i < chanel->invited.size(); ++i)
	{
		if (chanel->invited[i] == invited)
		{
			chanel->invited.erase(chanel->invited.begin() + i);
			return ;
		}
	}
}

