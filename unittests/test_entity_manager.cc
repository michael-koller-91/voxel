#include <boost/test/unit_test.hpp>

#include <bitset>
#include <iostream>

#include "entity.h"
#include "entity_manager.h"
#include "position.h"
#include "type.h"

BOOST_AUTO_TEST_CASE( create_and_destroy_entities )
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

BOOST_AUTO_TEST_CASE( register_and_add_components )
{
    struct TestComponent1
    {
        int a = 123.123;
    };

    struct TestComponent2
    {
        int b = 456.456;
    };

    EntityManager entity_manager;
    entity_manager.RegisterComponent<TestComponent1>();
    entity_manager.RegisterComponent<Position>();
    entity_manager.RegisterComponent<TestComponent2>();

    TestComponent1 test_component_1;
    Position position;
    TestComponent2 test_component_2;
    BOOST_CHECK_EQUAL(entity_manager.TypeIdOf(test_component_1), 0);
    BOOST_CHECK_EQUAL(entity_manager.TypeIdOf(position), 1);
    BOOST_CHECK_EQUAL(entity_manager.TypeIdOf(test_component_2), 2);

    // entity with only one component
    Entity ent1 = entity_manager.CreateEntity();
    TestComponent2 tc2_1;
    entity_manager.AddComponent(ent1, tc2_1);
    ComponentBitField bitfield1 = entity_manager.GetBitField(ent1);
    // bitfield1[2] must be true since TestComponent2 has type id 2
    BOOST_CHECK_EQUAL(bitfield1[0], false);
    BOOST_CHECK_EQUAL(bitfield1[1], false);
    BOOST_CHECK_EQUAL(bitfield1[2], true);
    BOOST_CHECK_EQUAL(bitfield1[3], false);

    // entity with two components
    Entity ent2 = entity_manager.CreateEntity();
    TestComponent1 tc1_2;
    TestComponent2 tc2_2;
    entity_manager.AddComponent(ent2, tc1_2);
    entity_manager.AddComponent(ent2, tc2_2);
    ComponentBitField bitfield2 = entity_manager.GetBitField(ent2);
    BOOST_CHECK_EQUAL(bitfield2[0], true);
    BOOST_CHECK_EQUAL(bitfield2[1], false);
    BOOST_CHECK_EQUAL(bitfield2[2], true);
    BOOST_CHECK_EQUAL(bitfield2[3], false);

    entity_manager.PrintEntityComponentBitfield();
}
