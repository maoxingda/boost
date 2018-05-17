// asio_tcp_server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
//#include <dbgmsg.h>
#include <atlbase.h>
#include "cat.h"

using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{
    try
    {
        if (2 != argc)
        {
            std::cout << "Please input listen port number! e.g. 18580" << std::endl;

            return -1;
        }

        boost::asio::io_service io_service;

        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), atoi(argv[1])));

        for (;;)
        {
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            boost::system::error_code ignored_error;

            char read_protocol_body_length[protocol_head_length] = { 0 };

            // read protocol head
            boost::asio::read(socket, boost::asio::buffer(read_protocol_body_length, protocol_head_length), ignored_error);

            u_long protocol_body_length = get_protocol_body_length(read_protocol_body_length);

            std::vector<char> cv_protocol_body(protocol_body_length, '\0');

            // read protocol body
            boost::asio::read(socket, boost::asio::buffer(&cv_protocol_body[0], protocol_body_length), ignored_error);

            std::string protocol_body(std::begin(cv_protocol_body), std::end(cv_protocol_body));

            std::cout << "recv " << protocol_body_length << " bytes data -> " << protocol_body << std::endl;

            std::string response_msg = concatenate_protocol_head(protocol_body, protocol_body.length());

            // response client info
            boost::asio::write(socket, boost::asio::buffer(response_msg, response_msg.length()), ignored_error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
