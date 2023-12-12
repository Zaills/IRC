#pragma once

#include "../Server.hpp"

void ERR_NORECIPIENT(client *ptr, std::string cmd_name);
void ERR_NOSUCHNICK(client *ptr);
void ERR_NOTEXTTOSEND(client *ptr);
void ERR_CANNOTSENDTOCHAN(client *ptr, std::string chan_name);
