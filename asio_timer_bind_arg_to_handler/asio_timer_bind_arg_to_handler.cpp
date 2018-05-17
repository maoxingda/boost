// asio_timer_bind_arg_to_handler.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <dbgmsg.h>

void print(const boost::system::error_code& /*e*/,
    boost::asio::deadline_timer* t, int* count)
{
    if (*count < 5)
    {
        //DBGMSG(_T("%d"), *count);
        ++(*count);

        t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
        t->async_wait(boost::bind(print,
            boost::asio::placeholders::error, t, count));
    }
}

int main()
{
    boost::asio::io_service io;

    int count = 0;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
    t.async_wait(boost::bind(print,
        boost::asio::placeholders::error, &t, &count));

    io.run();

    //DBGMSG(_T("Final count is %d."), count);

    return 0;
}