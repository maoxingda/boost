// bimap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <boost/bimap.hpp>


int main()
{
    boost::bimap<int, std::string> bimap1;

    // Insert data to bimap1 use left and right view
    bimap1.left.insert(std::make_pair(1, "毛兴达"));
    bimap1.right.insert(std::make_pair("孟宪伟", 2));

    // Find use left view by key 1
    auto lciter = bimap1.left.find(1);

    if (bimap1.left.end() != lciter)
    {
        printf("(key, value) = (%d, %s). \r\n", lciter->first, lciter->second.c_str());
    }

    // Find use right view by key 孟宪伟
    auto rciter = bimap1.right.find("孟宪伟");

    if (bimap1.right.end() != rciter)
    {
        printf("(key, value) = (%s, %d). \r\n", rciter->first.c_str(), rciter->second);
    }

    // Modify key or value use left and right view
    bimap1.left.replace_key(lciter, 3); // Modify key use left view

    lciter = bimap1.left.find(3);

    if (bimap1.left.end() != lciter)
    {
        printf("(key, value) = (%d, %s). \r\n", lciter->first, lciter->second.c_str());
    }

    // Modify value use left view
    bimap1.left.replace_data(lciter, "张成");

    lciter = bimap1.left.find(3);

    if (bimap1.left.end() != lciter)
    {
        printf("(key, value) = (%d, %s). \r\n", lciter->first, lciter->second.c_str());
    }

    // Modify key use right view
    bimap1.right.replace_key(rciter, "张亚鹏");

    rciter = bimap1.right.find("张亚鹏");

    if (bimap1.right.end() != rciter)
    {
        printf("(key, value) = (%s, %d). \r\n", rciter->first.c_str(), rciter->second);
    }

    // Modify value use right view
    bimap1.right.replace_data(rciter, 4);

    rciter = bimap1.right.find("张亚鹏");

    if (bimap1.right.end() != rciter)
    {
        printf("(key, value) = (%s, %d). \r\n", rciter->first.c_str(), rciter->second);
    }

    // Clear data
    bimap1.clear();

    assert(bimap1.empty());

    getchar();

    return 0;
}

