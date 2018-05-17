// asio_sync_timers.cpp : 定义控制台应用程序的入口点。
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
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <dbgmsg.h>

int main()
{
    boost::asio::io_service io;

    boost::asio::deadline_timer t(io, boost::posix_time::seconds(3));
    t.wait();

    //DBGMSG(_T("Hello world!"));

    return 0;
}