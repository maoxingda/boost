// asio_async_tcp_server.cpp : 定义控制台应用程序的入口点。
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
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "../asio_tcp_server/cat.h"
#include <thread>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

class tcp_connection
    : public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> tcp_connection_ptr;

    static tcp_connection_ptr create(boost::asio::io_service& io_service)
    {
        return tcp_connection_ptr(new tcp_connection(io_service));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        boost::system::error_code ignored_error;

        char read_protocol_body_length[protocol_head_length] = { 0 };

        // read protocol head
        boost::asio::read(socket_, boost::asio::buffer(read_protocol_body_length, protocol_head_length), ignored_error);

        u_long protocol_body_length = get_protocol_body_length(read_protocol_body_length);

        std::vector<char> cv_protocol_body(protocol_body_length, '\0');

        // read protocol body
        boost::asio::read(socket_, boost::asio::buffer(&cv_protocol_body[0], protocol_body_length), ignored_error);

        std::string protocol_body(std::begin(cv_protocol_body), std::end(cv_protocol_body));

        std::cout << "recv " << protocol_body_length << " bytes data -> " << protocol_body << std::endl;

        message_ = make_daytime_string();
        message_ = concatenate_protocol_head(message_, message_.length());

        boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

private:
    tcp_connection(boost::asio::io_service& io_service)
        : socket_(io_service)
    {
    }

    void handle_write(const boost::system::error_code& error,
        size_t bytes_transferred)
    {
        if (!error)
        {
            std::cout << "no error." << std::endl;
        }

        std::cout << "write " << bytes_transferred << " bytes data." << std::endl;

        std::cout << "thread_id -> " << ::GetCurrentThreadId() << std::endl;
    }

    void handle_read(const boost::system::error_code& error,
        size_t bytes_transferred)
    {
        if (!error)
        {
            std::cout << "no error." << std::endl;
        }

        std::cout << "read " << bytes_transferred << " bytes data." << std::endl;

        std::cout << "thread_id -> " << ::GetCurrentThreadId() << std::endl;
    }

    tcp::socket socket_;
    std::string message_;
};

class tcp_server
{
public:
    tcp_server(boost::asio::io_service& io_service, u_long listen_port)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), listen_port))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::tcp_connection_ptr new_connection =
            tcp_connection::create(acceptor_.get_io_service());

        acceptor_.async_accept(new_connection->socket(),
            boost::bind(&tcp_server::handle_accept, this, new_connection,
                boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::tcp_connection_ptr new_connection,
        const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }

        start_accept();
    }

    tcp::acceptor acceptor_;
};

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
        tcp_server server(io_service, atoi(argv[1]));

        //std::thread thrd1([&] { io_service.run(); });

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}