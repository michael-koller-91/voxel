#include <boost/test/unit_test.hpp>

#include <bitset>
#include <iostream>

#include "entity.h"
#include "entity_manager.h"
#include "type.h"

BOOST_AUTO_TEST_CASE( create_destroy_entities )
{
    EntityManager entity_manager;

    // create some entities and check if the counter is correct
    Entity ent1 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 1);

    Entity ent2 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 2);

    Entity ent3 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 3);

    // destroy some entities and check if the counter is correct
    entity_manager.DestroyEntity(ent1);
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 2);

    entity_manager.DestroyEntity(ent2);
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 1);

    // create new entities (reusing destroyed ones)
    Entity ent4 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 2);

    Entity ent5 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 3);

    Entity ent6 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 4);
}

BOOST_AUTO_TEST_CASE( register_add_get_components )
{
    struct TestComponent0
    {
        int a = -1;
    };

    struct TestComponent1
    {
        float b = -1;
    };

    struct TestComponent2
    {
        bool c = false;
    };

    /* register components and check assigned ID */
    EntityManager entity_manager;
    entity_manager.RegisterComponent<TestComponent0>();
    entity_manager.RegisterComponent<TestComponent1>();
    entity_manager.RegisterComponent<TestComponent2>();
    BOOST_CHECK_EQUAL(entity_manager.TypeIdOf<TestComponent0>(), 0);
    BOOST_CHECK_EQUAL(entity_manager.TypeIdOf<TestComponent1>(), 1);
    BOOST_CHECK_EQUAL(entity_manager.TypeIdOf<TestComponent2>(), 2);

    /* add components to entities and check bit field entries */
    // entity without components
    Entity ent0 = entity_manager.CreateEntity();
    ComponentBitField bitfield0 = entity_manager.GetBitField(ent0);
    ComponentBitField expected_bitfield0;
    BOOST_CHECK_EQUAL(bitfield0, expected_bitfield0);

    // entity with only one component
    Entity ent1 = entity_manager.CreateEntity();
    entity_manager.AddComponent(ent1, TestComponent2{true});
    ComponentBitField bitfield1 = entity_manager.GetBitField(ent1);
    ComponentBitField expected_bitfield1;
    expected_bitfield1.set(2);  // since TestComponent2 has type id 2
    BOOST_CHECK_EQUAL(bitfield1, expected_bitfield1);

    // entity with two components
    Entity ent2 = entity_manager.CreateEntity();
    entity_manager.AddComponent(ent2, TestComponent0{20});
    entity_manager.AddComponent(ent2, TestComponent2{true});
    ComponentBitField bitfield2 = entity_manager.GetBitField(ent2);
    ComponentBitField expected_bitfield2;
    expected_bitfield2.set(0);
    expected_bitfield2.set(2);
    BOOST_CHECK_EQUAL(bitfield2, expected_bitfield2);

    // entity with three components (by adding one more component to ent2)
    entity_manager.AddComponent(ent2, TestComponent1{21});
    ComponentBitField bitfield3 = entity_manager.GetBitField(ent2);
    ComponentBitField expected_bitfield3;
    expected_bitfield3.set(0);
    expected_bitfield3.set(1);
    expected_bitfield3.set(2);
    BOOST_CHECK_EQUAL(bitfield3, expected_bitfield3);

    /* get component bit fields */
    // bit field without components
    ComponentBitField cbf = entity_manager.ComponentBitFieldOf<>();
    ComponentBitField expected_cbf;
    BOOST_CHECK_EQUAL(cbf, expected_cbf);

    // bit field with one component
    cbf = entity_manager.ComponentBitFieldOf<TestComponent1>();
    expected_cbf.reset();
    expected_cbf.set(1);
    BOOST_CHECK_EQUAL(cbf, expected_cbf);

    // bit field with two components
    cbf = entity_manager.ComponentBitFieldOf<TestComponent0, TestComponent2>();
    expected_cbf.reset();
    expected_cbf.set(0);
    expected_cbf.set(2);
    BOOST_CHECK_EQUAL(cbf, expected_cbf);

    // bit field with three components
    cbf = entity_manager.ComponentBitFieldOf<TestComponent0, TestComponent1, TestComponent2>();
    expected_cbf.reset();
    expected_cbf.set(0);
    expected_cbf.set(1);
    expected_cbf.set(2);
    BOOST_CHECK_EQUAL(cbf, expected_cbf);

    /* get components */
    // entity with one component
    Entity ent1comp = entity_manager.CreateEntity();
    entity_manager.AddComponent(ent1comp, TestComponent0{123});
    auto& e1tc0 = entity_manager.GetComponent<TestComponent0>(ent1comp);
    BOOST_CHECK_EQUAL(e1tc0.a, 123);
    // modify component
    e1tc0.a *= 2;
    BOOST_CHECK_EQUAL(entity_manager.GetComponent<TestComponent0>(ent1comp).a, 246);

    // entity with two components
    Entity ent2comp = entity_manager.CreateEntity();
    entity_manager.AddComponent(ent2comp, TestComponent0{123});
    entity_manager.AddComponent(ent2comp, TestComponent2{true});
    auto& e2tc0 = entity_manager.GetComponent<TestComponent0>(ent2comp);
    auto& e2tc2 = entity_manager.GetComponent<TestComponent2>(ent2comp);
    BOOST_CHECK_EQUAL(e2tc0.a, 123);
    BOOST_CHECK_EQUAL(e2tc2.c, true);
}
