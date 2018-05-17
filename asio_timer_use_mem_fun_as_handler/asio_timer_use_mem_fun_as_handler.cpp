// asio_timer_use_mem_fun_as_handler.cpp : 定义控制台应用程序的入口点。
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

class printer
{
public:
    printer(boost::asio::io_service& io)
        : timer_(io, boost::posix_time::seconds(1)),
        count_(0)
    {
        timer_.async_wait(boost::bind(&printer::print, this));
    }

    ~printer()
    {
        //DBGMSG(_T("Final count is %d."), count_);
    }

    void print()
    {
        if (count_ < 5)
        {
            //DBGMSG(_T("%d"), count_);
            ++count_;

            timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
            timer_.async_wait(boost::bind(&printer::print, this));
        }
    }

private:
    boost::asio::deadline_timer timer_;
    int count_;
};

int main()
{
    boost::asio::io_service io;
    printer p(io);
    io.run();

    return 0;
}