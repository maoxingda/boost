// asio_tcp_client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
//#include <dbgmsg.h>
#include <atlbase.h>
#include "../asio_tcp_server/cat.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        if (3 != argc)
        {
            std::cout << "Please input server ip and port number! e.g. 127.0.0.1 18580" << std::endl;

            return -1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({ argv[1], argv[2] });

        char line[256] = { 0 };
        while (true)
            //while (std::cin.getline(line, 256))
        {
            tcp::socket socket(io_service);
            boost::asio::connect(socket, endpoint_iterator);

            boost::system::error_code ec;

            strcat(line, "123");

            std::string send = concatenate_protocol_head(line, strlen(line));

            // send request protocol
            boost::asio::write(socket, boost::asio::buffer(send, send.length()), ec);

            char read_protocol_body_length[protocol_head_length] = { 0 };

            // read response protocol head
            boost::asio::read(socket, boost::asio::buffer(read_protocol_body_length, protocol_head_length), ec);

            u_long protocol_body_length = get_protocol_body_length(read_protocol_body_length);

            std::vector<char> cv(protocol_body_length, '\0');

            // read response protocol body
            boost::asio::read(socket, boost::asio::buffer(&cv[0], protocol_body_length), ec);

            std::string protocol_body(std::begin(cv), std::end(cv));

            std::cout << "recv " << protocol_body_length << " bytes data -> " << protocol_body << std::endl;

            ::ZeroMemory(line, sizeof(line));
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
