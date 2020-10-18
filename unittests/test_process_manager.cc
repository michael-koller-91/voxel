#include <boost/test/unit_test.hpp>

#include "process.h"
#include "process_manager.h"

class TestProcess0 : public IProcess
{
public:
    void update()
    {
        ++a0;
    }
    int a0 = 1;
};

class TestProcess1 : public IProcess
{
public:
    void update()
    {
        --a1;
    }
    int a1 = -1;
};

BOOST_AUTO_TEST_CASE( register_get_update_processes )
{
    /* register processes */

    ProcessManager process_manager;
    process_manager.RegisterProcess<TestProcess0>(0);
    process_manager.RegisterProcess<TestProcess1>(1);

    /* get priority of processes */

    BOOST_CHECK_EQUAL(process_manager.PriorityOf<TestProcess0>(), 0);
    BOOST_CHECK_EQUAL(process_manager.PriorityOf<TestProcess1>(), 1);

    /* get processes */

    auto tp0 = process_manager.GetProcess<TestProcess0>();
    auto tp1 = process_manager.GetProcess<TestProcess1>();
    BOOST_CHECK_EQUAL(tp0->a0, 1);
    BOOST_CHECK_EQUAL(tp1->a1, -1);

    /* update processes */

    process_manager.update();
    BOOST_CHECK_EQUAL(tp0->a0, 2);
    BOOST_CHECK_EQUAL(tp1->a1, -2);
    process_manager.update();
    BOOST_CHECK_EQUAL(tp0->a0, 3);
    BOOST_CHECK_EQUAL(tp1->a1, -3);
    process_manager.update();

    /* get processes again */

    auto tp2 = process_manager.GetProcess<TestProcess0>();
    auto tp3 = process_manager.GetProcess<TestProcess1>();
    BOOST_CHECK_EQUAL(tp2->a0, 4);
    BOOST_CHECK_EQUAL(tp3->a1, -4);
}
