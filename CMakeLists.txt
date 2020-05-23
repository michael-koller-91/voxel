cmake_minimum_required (VERSION 3.16)
project (
    voxel
    VERSION 0.1
    DESCRIPTION "A voxel game."
    LANGUAGES CXX
)

add_subdirectory (src)
add_subdirectory (app)

enable_testing ()
add_subdirectory (unittests)
