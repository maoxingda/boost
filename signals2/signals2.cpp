// signals2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <boost/signals2.hpp>
//#include <dbgmsg.h>


void single_slot()
{
    boost::signals2::signal<void()> sig;

    auto hello_world = []() { /*DBGMSG(_T("Hello world!"));*/ };

    sig.connect(hello_world);

    sig();
}

void multiple_slots()
{
    boost::signals2::signal<void()> sig;

    auto hello = []() { /*::OutputDebugString(_T("Hello "));*/ };
    auto world = []() { /*::OutputDebugString(_T("world! \r\n"));*/ };

    sig.connect(hello);
    sig.connect(world);

    sig();
}

// slots are invoked this order:
// 1) ungrouped slots connected with boost::signals2::at_front
// 2) grouped slots according to ordering of their groups
// 3) ungrouped slots connected with boost::signals2::at_back

void ordering_call_slot_groups()
{
    boost::signals2::signal<void()> sig;

    auto hello        = []() { /*::OutputDebugString(_T("Hello "));*/ };
    auto world        = []() { /*::OutputDebugString(_T("world! \r\n"));*/ };
    auto good_morning = []() { /*::OutputDebugString(_T("... and good morning! \r\n"));*/ };

    sig.connect(2, world);
    sig.connect(1, hello);

    sig.connect(good_morning);

    sig();
}

void slot_arguments()
{
    boost::signals2::signal<void(float, float)> sig;

    auto args = [](float x, float y) { /*DBGMSG(_T("The arguments are %lf and %lf."), x, y);*/ };
    auto sum  = [](float x, float y) { /*DBGMSG(_T("The sum is %lf."), x + y);*/ };
    auto sub  = [](float x, float y) { /*DBGMSG(_T("The subtract is %lf."), x - y);*/ };
    auto pro  = [](float x, float y) { /*DBGMSG(_T("The production is %lf."), x * y);*/ };
    auto quo  = [](float x, float y) { /*DBGMSG(_T("The quotient is %lf."), x / y);*/ };

    sig.connect(args);

    sig.connect(sum);
    sig.connect(sub);
    sig.connect(pro);
    sig.connect(quo);

    sig(5, 3);
}

int main()
{
    single_slot();

    multiple_slots();

    ordering_call_slot_groups();

    slot_arguments();

    return 0;
}

