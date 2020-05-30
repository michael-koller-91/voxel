#include <boost/test/unit_test.hpp>
#include <bitset>

#include "entity_manager.h"
#include "position.h"
#include "type.h"

BOOST_AUTO_TEST_CASE( creating_and_destroying_entities )
{
    EntityManager entity_manager;

    // create some entities and check if the counter is correct
    Entity ent1 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 1);

    Entity ent2 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entity_manager.GetExistingEntitiesCount(), 2);

    Entity ent2 = entity_manager.CreateEntity();
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
    BOOST_CHECK_EQUAL(entitymanager.GetExistingEntitiesCount(), 3);

    Entity ent6 = entity_manager.CreateEntity();
    BOOST_CHECK_EQUAL(entitymanager.GetExistingEntitiesCount(), 4);
}
