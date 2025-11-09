cmake_minimum_required(VERSION 3.10.0)
project(project2_animals VERSION 0.1.0 LANGUAGES C CXX)

add_definitions(-D_WIN32_WINNT=0x0A00)

add_executable(project2_animals main.cpp Load.cpp Taxon.cpp)
target_link_libraries(project2_animals ws2_32)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
