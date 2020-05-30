project (src)

add_library (${PROJECT_NAME} STATIC
	entity_manager.cc
)

add_library (libs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})
