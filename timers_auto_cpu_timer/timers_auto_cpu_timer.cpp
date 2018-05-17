// timers_auto_cpu_timer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>
#include <cmath>
//#include <dbgmsg.h>
#include <atlbase.h>


int main()
{
    std::ostringstream oss;

    {
        boost::timer::auto_cpu_timer t(oss);

        for (long i = 0; i < 100000000; ++i)
        {
            std::sqrt(123.456L);                // burn some time
        }
    }

    //DBGMSG(_T("%s."), (LPCTSTR)CA2T(oss.str().c_str()));

    return 0;
}