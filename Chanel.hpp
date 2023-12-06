/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chanel.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 14:22:56 by marvin            #+#    #+#             */
/*   Updated: 2023/12/05 14:22:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANEL_HPP
# define CHANEL_HPP

# include <string>
# include <vector>

typedef struct s_chanel{

	std::string	name;
	std::string	password;
	std::string	topic;
	std::vector<std::string>	user;
	std::vector<std::string>	admin;

	unsigned int	user_limit;

	std::vector<std::string>	invited;
	bool	invite;

}	Chanel;

Chanel	creat_chanel(std::string name, std::string password, std::string admin);

void	del_user(Chanel *chanel, std::string user);
void	del_admin(Chanel *chanel, std::string admin);
void	del_invited(Chanel *chanel, std::string invited);

#endif
