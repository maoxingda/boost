// lambda_introductory_examples.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <list>
#include <vector>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

//#include <dbgmsg.h>

int double_value(int arg1)
{
    return 2 * arg1;
}

int main()
{
    std::list<int> il(3);

    std::for_each(std::begin(il), std::end(il), boost::lambda::_1 = 1);

    for (const auto& e : il)
    {
        //DBGMSG(_T("%d"), e);
    }

    //////////////////////////////////////////////////////////////////////////
    std::vector<int*> ipv(3);
    std::transform(std::begin(il), std::end(il), std::begin(ipv), &(boost::lambda::_1));

    for (const auto& pe : ipv)
    {
        //DBGMSG(_T("%d"), *pe);
    }

    //////////////////////////////////////////////////////////////////////////
/*
    std::for_each(std::begin(il), std::end(il), boost::lambda::_1 = boost::bind(&double_value, boost::lambda::_1));
*/

}

