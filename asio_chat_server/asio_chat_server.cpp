// asio_chat_server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "chat_message.hpp"
#include <atlbase.h>
//#include <dbgmsg.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_deque;

//----------------------------------------------------------------------

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
    virtual boost::uuids::uuid participant_id() = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:
    void join(chat_participant_ptr participant)
    {
        //DBGMSG(_T("in."));

        participants_.insert(participant);

        for (auto msg : recent_msgs_)
        {
            participant->deliver(msg);
        }
    }

    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);
    }

    void deliver(const chat_message& msg, boost::uuids::uuid session_id)
    {
        //DBGMSG(_T("in."));

        recent_msgs_.push_back(msg);

        while (recent_msgs_.size() > max_recent_msgs)
        {
            recent_msgs_.pop_front();
        }

        for (auto participant : participants_)
        {
            // send message to other participants other than self.
            if (session_id != participant->participant_id())
            {
                participant->deliver(msg);
            }
        }
    }

private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    chat_message_deque recent_msgs_;
};

//----------------------------------------------------------------------

class chat_session
    : public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room& room)
        : socket_(std::move(socket)),
        room_(room),
        session_id_(boost::uuids::random_generator()())
    {
    }

    void start()
    {
        //DBGMSG(_T("in."));
        room_.join(shared_from_this());
        do_read_header();
    }

    void deliver(const chat_message& msg)
    {
        //DBGMSG(_T("in."));
        write_msgs_.push_back(msg);
        if (!write_msgs_.empty())
        {
            do_write();
        }
    }

    virtual boost::uuids::uuid participant_id()
    {
        return session_id_;
    }

private:
    void do_read_header()
    {
        //DBGMSG(_T("in."));
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.data(), chat_message::header_length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            //DBGMSG(_T("in."));
            if (!ec && read_msg_.decode_header())
            {
                do_read_body();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    void do_read_body()
    {
        //DBGMSG(_T("in."));
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
            boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            //DBGMSG(_T("in."));
            if (!ec)
            {
                room_.deliver(read_msg_, session_id_);

                //DBGMSG(_T("Recv data: %s."), (LPCTSTR)CA2T(read_msg_.data() + read_msg_.header_length));

                do_read_header();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    void do_write()
    {
        //DBGMSG(_T("in."));
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_msgs_.front().data(),
                write_msgs_.front().length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            //DBGMSG(_T("in."));
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    do_write();
                }
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_deque write_msgs_;
    boost::uuids::uuid session_id_;
};

//----------------------------------------------------------------------

class chat_server
{
public:
    chat_server(boost::asio::io_service& io_service,
        const tcp::endpoint& endpoint)
        : acceptor_(io_service, endpoint),
        socket_(io_service)
    {
        //DBGMSG(_T("in."));

        do_accept();
    }

private:
    void do_accept()
    {
        //DBGMSG(_T("in."));
        acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec)
        {
            //DBGMSG(_T("in."));
            if (!ec)
            {
                std::make_shared<chat_session>(std::move(socket_), room_)->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    chat_room room_;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }

        boost::asio::io_service io_service;

        std::list<chat_server> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_service, endpoint);
        }

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}