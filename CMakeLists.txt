cmake_minimum_required (VERSION 3.16)
project (voxel
    VERSION 0.1
    DESCRIPTION "A voxel game."
    LANGUAGES CXX)

# find_package(Boost 1.73.0)
# if(Boost_FOUND)
#     include_directories(${Boost_INCLUDE_DIRS})
#     # add_executable(foo foo.cc)
# endif()

add_subdirectory (src)
add_subdirectory (app)

enable_testing ()
add_subdirectory (test)
