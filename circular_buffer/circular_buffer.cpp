// circular_buffer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <boost/circular_buffer.hpp>


int main()
{
    // Create a circular buffer with a capacity for 3 string.
    boost::circular_buffer<std::string> cb(3);

    // Insert threee elements into the buffer.
    cb.push_back("1");
    cb.push_back("2");
    cb.push_back("3");

    std::string a = cb[0];  // a == 1
    std::string b = cb[1];  // b == 2
    std::string c = cb[2];  // c == 3

                    // The buffer is full now, so pushing subsequent
                    // elements will overwrite the front-most elements.

    cb.push_back("4");  // Overwrite 1 with 4.
    cb.push_back("5");  // Overwrite 2 with 5.

                      // The buffer now contains 3, 4 and 5.
    a = cb[0];  // a == 3
    b = cb[1];  // b == 4
    c = cb[2];  // c == 5

                // Elements can be popped from either the front or the back.
    cb.pop_back();  // 5 is removed.
    cb.pop_front(); // 3 is removed.

                    // Leaving only one element with value = 4.
    std::string d = cb[0];  // d == 4}
}
