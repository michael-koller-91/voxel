#include <boost/test/unit_test.hpp>

#include <bitset>
#include <iostream>

#include "entity.h"
#include "entity_manager.h"
#include "type.h"

namespace test_entity_manager_namespace{
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
}

BOOST_AUTO_TEST_CASE( create_and_destroy_entities )
{
    using namespace test_entity_manager_namespace;

    EntityManager entity_manager;

    /* create some entities and check if the counter is correct */

    Entity ent1 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 1);

    Entity ent2 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 2);

    Entity ent3 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 3);

    /* destroy some entities and check if the counter is correct */

    entity_manager.DestroyEntity(ent1);
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 2);

    entity_manager.DestroyEntity(ent2);
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 1);

    /* create new entities (reusing destroyed ones) */

    Entity ent4 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 2);

    Entity ent5 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 3);

    Entity ent6 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 4);
}

BOOST_AUTO_TEST_CASE( register_add_and_remove_components )
{
    using namespace test_entity_manager_namespace;

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

    // entity with three components
    Entity ent3 = entity_manager.CreateEntity();
    entity_manager.AddComponent(ent3, TestComponent0{20});
    entity_manager.AddComponent(ent3, TestComponent1{21});
    entity_manager.AddComponent(ent3, TestComponent2{true});
    ComponentBitField bitfield3 = entity_manager.GetBitField(ent3);
    ComponentBitField expected_bitfield3;
    expected_bitfield3.set(0);
    expected_bitfield3.set(1);
    expected_bitfield3.set(2);
    BOOST_CHECK_EQUAL(bitfield3, expected_bitfield3);

    /* remove components from entities and check bit field entries */

    // remove the only component
    entity_manager.RemoveComponent<TestComponent2>(ent1);
    bitfield1 = entity_manager.GetBitField(ent1);
    BOOST_CHECK_EQUAL(bitfield1, ComponentBitField());

    // remove one of two components
    entity_manager.RemoveComponent<TestComponent0>(ent2);
    bitfield2 = entity_manager.GetBitField(ent2);
    expected_bitfield2.reset(0);
    BOOST_CHECK_EQUAL(bitfield2, expected_bitfield2);

    // remove two of three components
    entity_manager.RemoveComponent<TestComponent0>(ent3);
    bitfield3 = entity_manager.GetBitField(ent3);
    expected_bitfield3.reset(0);
    BOOST_CHECK_EQUAL(bitfield3, expected_bitfield3);
    entity_manager.RemoveComponent<TestComponent1>(ent3);
    bitfield3 = entity_manager.GetBitField(ent3);
    expected_bitfield3.reset(1);
    BOOST_CHECK_EQUAL(bitfield3, expected_bitfield3);
}

BOOST_AUTO_TEST_CASE( get_components_and_entity_component_map )
{
    using namespace test_entity_manager_namespace;

    EntityManager entity_manager;
    entity_manager.RegisterComponent<TestComponent0>();
    entity_manager.RegisterComponent<TestComponent1>();
    entity_manager.RegisterComponent<TestComponent2>();
    BOOST_CHECK_EQUAL(entity_manager.TypeIdOf<TestComponent0>(), 0);
    BOOST_CHECK_EQUAL(entity_manager.TypeIdOf<TestComponent1>(), 1);
    BOOST_CHECK_EQUAL(entity_manager.TypeIdOf<TestComponent2>(), 2);

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

    /* get components and check entity component map */

    // size of entity component maps must be 0 when no components have been added
    auto cm_tc0 = entity_manager.GetComponentMap<TestComponent0>();
    auto cm_tc1 = entity_manager.GetComponentMap<TestComponent1>();
    auto cm_tc2 = entity_manager.GetComponentMap<TestComponent2>();
    BOOST_CHECK_EQUAL(cm_tc0->entity_component_map_.size(), 0);
    BOOST_CHECK_EQUAL(cm_tc1->entity_component_map_.size(), 0);
    BOOST_CHECK_EQUAL(cm_tc2->entity_component_map_.size(), 0);

    // entity with one component
    Entity ent1comp = entity_manager.CreateEntity();
    entity_manager.AddComponent(ent1comp, TestComponent0{123});
    BOOST_CHECK_EQUAL(cm_tc0->entity_component_map_.size(), 1);
    auto& e1tc0 = entity_manager.GetComponent<TestComponent0>(ent1comp);
    BOOST_CHECK_EQUAL(e1tc0.a, 123);
    e1tc0.a *= 2;   // modify component
    BOOST_CHECK_EQUAL(entity_manager.GetComponent<TestComponent0>(ent1comp).a, 246);

    // entity with two components
    Entity ent2comp = entity_manager.CreateEntity();
    TestComponent0 tc0 = TestComponent0{123};
    entity_manager.AddComponent(ent2comp, tc0);
    BOOST_CHECK_EQUAL(cm_tc0->entity_component_map_.size(), 2);
    entity_manager.AddComponent(ent2comp, TestComponent2{true});
    BOOST_CHECK_EQUAL(cm_tc2->entity_component_map_.size(), 1);
    auto& e2tc0 = entity_manager.GetComponent<TestComponent0>(ent2comp);
    auto& e2tc2 = entity_manager.GetComponent<TestComponent2>(ent2comp);
    BOOST_CHECK_EQUAL(e2tc0.a, 123);
    BOOST_CHECK_EQUAL(e2tc2.c, true);

    /* remove components and check entity component map */

    entity_manager.RemoveComponent<TestComponent0>(ent1comp);
    BOOST_CHECK_EQUAL(cm_tc0->entity_component_map_.size(), 1);
    entity_manager.RemoveComponent(ent2comp, tc0);  // test the other version of RemoveComponent
    BOOST_CHECK_EQUAL(cm_tc0->entity_component_map_.size(), 0);
    entity_manager.RemoveComponent<TestComponent2>(ent2comp);
    BOOST_CHECK_EQUAL(cm_tc2->entity_component_map_.size(), 0);
}
