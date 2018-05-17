#include "stdafx.h"
#include "cat.h"

std::string concatenate_protocol_head(std::string protocol, u_long protocol_length)
{
    protocol_length = htonl(protocol_length);

    byte* plength   = reinterpret_cast<byte*>(&protocol_length);

    for (int i = protocol_head_length - 1; i >= 0; --i)
    {
        protocol.insert(protocol.begin(), plength[i]);
    }

    return protocol;
}

u_long get_protocol_body_length(const char* protocol_head)
{
    u_long protocol_lentgh = 0;

    byte* plength          = reinterpret_cast<byte*>(&protocol_lentgh);

    for (size_t i = 0; i < protocol_head_length; ++i)
    {
        plength[i] = protocol_head[i];
    }

    return ntohl(protocol_lentgh);
}
