project(unittests)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.73.0 COMPONENTS unit_test_framework REQUIRED)

include_directories (
    ${CMAKE_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)

add_executable (unittests
    main.cc
    test_entity_manager.cc
)
