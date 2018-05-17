#pragma once

#include <string>
#include <WinSock2.h>

#define protocol_head_length 4ul

std::string concatenate_protocol_head(std::string protocol, u_long protocol_length);

u_long      get_protocol_body_length(const char* protocol_head);