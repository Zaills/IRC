#pragma once

#include <string>

typedef struct s_client{
	std::string nick;
	std::string user;
	std::string password;
	bool is_logged;
}	client;
