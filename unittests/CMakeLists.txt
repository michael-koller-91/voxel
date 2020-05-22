project (unittests)

set (Boost_USE_STATIC_LIBS ON)
find_package (Boost 1.73.0 COMPONENTS unit_test_framework REQUIRED)

include_directories (
    ${CMAKE_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)

add_executable (${PROJECT_NAME}
    testmain.cc
    test_component_manager.cc
    test_entity_manager.cc
)
target_link_libraries (${PROJECT_NAME} libs::src)
