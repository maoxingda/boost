// array.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <algorithm>
#include <iostream>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>


int main()
{
    boost::array<int, 3> ia = { 1, 2, 3 };

    std::for_each(std::begin(ia), std::end(ia), std::cout << boost::lambda::_1 << " ");

    getchar();

    return 0;
}
