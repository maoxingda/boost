// asio_timer_sync_handlers_in_multithread_programs.cpp : 定义控制台应用程序的入口点。
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
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <dbgmsg.h>

class printer
{
public:
    printer(boost::asio::io_service& io)
        : strand_(io),
        timer1_(io, boost::posix_time::seconds(1)),
        timer2_(io, boost::posix_time::seconds(1)),
        count_(0)
    {
        timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
        timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
    }

    ~printer()
    {
        //DBGMSG(_T("Final count is %d."), count_);
    }

    void print1()
    {
        if (count_ < 10)
        {
            //DBGMSG(_T("Timer 1: "), count_);
            ++count_;

            timer1_.expires_at(timer1_.expires_at() + boost::posix_time::seconds(1));
            timer1_.async_wait(strand_.wrap(boost::bind(&printer::print1, this)));
        }
    }

    void print2()
    {
        if (count_ < 10)
        {
            //DBGMSG(_T("Timer 2: "), count_);
            ++count_;

            timer2_.expires_at(timer2_.expires_at() + boost::posix_time::seconds(1));
            timer2_.async_wait(strand_.wrap(boost::bind(&printer::print2, this)));
        }
    }

private:
    boost::asio::io_service::strand strand_;
    boost::asio::deadline_timer timer1_;
    boost::asio::deadline_timer timer2_;
    int count_;
};

int main()
{
    boost::asio::io_service io;
    printer p(io);
    boost::thread t(boost::bind(&boost::asio::io_service::run, &io));
    io.run();
    t.join();

    return 0;
}