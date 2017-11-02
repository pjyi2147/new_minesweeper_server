cmake_minimum_required (VERSION 3.5)
project (minesweeper_server)

find_package(Boost REQUIRED COMPONENTS system)

file(GLOB MAIN src/main.cpp)
file(GLOB COMMON src/common/*.cpp)
file(GLOB SERVER src/server/*.cpp)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(m_server ${MAIN} ${COMMON} ${SERVER})
    target_link_libraries(m_server ${Boost_LIBRARIES})
endif()