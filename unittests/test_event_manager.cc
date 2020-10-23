#include <boost/test/unit_test.hpp>

#include "event_manager.h"
#include "process.h"
#include "process_manager.h"

namespace test_event_manager_namespace {
    /* test events */
    struct IncreaseA0
    {
        int a0_increment = 5;
    };

    struct IncreaseB0
    {
        int b0_increment = 10;
    };

    struct DecreaseB1
    {
        float b1_decrement = 20.0;
    };

    struct Dummy
    {
        bool x = true;
    };

    /* test processes */
    class TestProcess0 : public IProcess
    {
    public:
        void Update()
        {
            ++a0;
            ++b0;
        }

        void Receive(IncreaseA0& e)
        {
            a0 += e.a0_increment;
        }

        int a0 = 1;
        int b0 = 100;
    };

    class TestProcess1 : public IProcess
    {
    public:
        void Update()
        {
            ++a0;
            ++b0;
            b1 -= 1.0;
        }

        void Receive(IncreaseA0& e)
        {
            a0 += e.a0_increment;
        }

        void Receive(IncreaseB0& e)
        {
            b0 += e.b0_increment;
        }

        void Receive(DecreaseB1& e)
        {
            b1 -= e.b1_decrement;
        }

        int a0 = 0;
        int b0 = 100;
        float b1 = -1.0;
    };
}

BOOST_AUTO_TEST_CASE( add_event_callbacks_and_publish )
{
    using namespace test_event_manager_namespace;

    EventManager event_manager;
    ProcessManager process_manager;

    // process with only one receive method
    process_manager.RegisterProcess<TestProcess0>(0);
    // process with multiple receive methods
    process_manager.RegisterProcess<TestProcess1>(1);

    auto tp0 = process_manager.GetProcess<TestProcess0>();
    auto tp1 = process_manager.GetProcess<TestProcess1>();

    /* subscribe */

    // two subscribers for event IncreaseA0
    event_manager.Subscribe<IncreaseA0>(tp0);
    event_manager.Subscribe<IncreaseA0>(tp1);
    // one subscriber for the other events
    event_manager.Subscribe<IncreaseB0>(tp1);
    event_manager.Subscribe<DecreaseB1>(tp1);

    auto cb_a0 = event_manager.GetCallbacks<IncreaseA0>();
    auto cb_b0 = event_manager.GetCallbacks<IncreaseB0>();
    auto cb_b1 = event_manager.GetCallbacks<DecreaseB1>();
    BOOST_CHECK_EQUAL(cb_a0.size(), 2);
    BOOST_CHECK_EQUAL(cb_b0.size(), 1);
    BOOST_CHECK_EQUAL(cb_b1.size(), 1);

    /* publish events */

    // make sure registering event callbacks didn't change the processes' initial states
    BOOST_CHECK_EQUAL(tp0->a0, 1);
    BOOST_CHECK_EQUAL(tp0->b0, 100);
    BOOST_CHECK_EQUAL(tp1->a0, 0);
    BOOST_CHECK_EQUAL(tp1->b0, 100);
    BOOST_CHECK_EQUAL(tp1->b1, -1.0);

    // publish event with multiple receivers
    event_manager.Publish(IncreaseA0({9}));
    BOOST_CHECK_EQUAL(tp0->a0, 10);
    BOOST_CHECK_EQUAL(tp0->b0, 100);
    BOOST_CHECK_EQUAL(tp1->a0, 9);
    BOOST_CHECK_EQUAL(tp1->b0, 100);
    BOOST_CHECK_EQUAL(tp1->b1, -1.0);

    // publish event with only one receiver
    event_manager.Publish(IncreaseB0());
    BOOST_CHECK_EQUAL(tp0->a0, 10);
    BOOST_CHECK_EQUAL(tp0->b0, 100);    // tp0 did not subscribe to IncreaseB0 events
    BOOST_CHECK_EQUAL(tp1->a0, 9);
    BOOST_CHECK_EQUAL(tp1->b0, 110);
    BOOST_CHECK_EQUAL(tp1->b1, -1.0);

    event_manager.Publish(DecreaseB1({50.0}));
    BOOST_CHECK_EQUAL(tp0->a0, 10);
    BOOST_CHECK_EQUAL(tp0->b0, 100);
    BOOST_CHECK_EQUAL(tp1->a0, 9);
    BOOST_CHECK_EQUAL(tp1->b0, 110);
    BOOST_CHECK_EQUAL(tp1->b1, -51.0);

    process_manager.Update();
    BOOST_CHECK_EQUAL(tp0->a0, 11);
    BOOST_CHECK_EQUAL(tp0->b0, 101);
    BOOST_CHECK_EQUAL(tp1->a0, 10);
    BOOST_CHECK_EQUAL(tp1->b0, 111);
    BOOST_CHECK_EQUAL(tp1->b1, -52.0);

    /* unsubscribe */

    event_manager.Unsubscribe<IncreaseA0>(tp1);
    // unsubscribe twice, nothing should happen the second time
    event_manager.Unsubscribe<IncreaseA0>(tp1);

    event_manager.Publish(IncreaseA0({3}));
    BOOST_CHECK_EQUAL(tp0->a0, 14);
    BOOST_CHECK_EQUAL(tp0->b0, 101);
    BOOST_CHECK_EQUAL(tp1->a0, 10);     // tp1 unsubscribed and thus should not be updated
    BOOST_CHECK_EQUAL(tp1->b0, 111);
    BOOST_CHECK_EQUAL(tp1->b1, -52.0);

    // tp0 never subscribed to DecreaseB1, so nothing should happen
    event_manager.Unsubscribe<DecreaseB1>(tp0);

    // no process subscribed to Dummy (in particular not tp0), so nothing should happen
    event_manager.Unsubscribe<Dummy>(tp0);

    /* re-subscribe */

    event_manager.Subscribe<IncreaseA0>(tp1);
    event_manager.Publish(IncreaseA0({8}));
    BOOST_CHECK_EQUAL(tp0->a0, 22);
    BOOST_CHECK_EQUAL(tp0->b0, 101);
    BOOST_CHECK_EQUAL(tp1->a0, 18);     // tp1 re-subscribed
    BOOST_CHECK_EQUAL(tp1->b0, 111);
    BOOST_CHECK_EQUAL(tp1->b1, -52.0);
}
