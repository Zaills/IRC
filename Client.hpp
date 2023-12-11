#pragma once

#include <string>

typedef struct s_client{
	std::string nick;
	std::string user;
	std::string password;
	int	fd;
	bool is_logged;
}	client;
