// thread.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//#include <dbgmsg.h>
#include <boost/thread.hpp>


int g_nNumber = 0;

void increment()
{
    ++g_nNumber;
}

int main()
{
    boost::thread thrd1(&increment);
    boost::thread thrd2(&increment);

    thrd1.join();
    thrd2.join();

    //DBGMSG(_T("%d."), g_nNumber);

    return 0;
}

