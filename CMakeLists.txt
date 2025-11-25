cmake_minimum_required(VERSION 3.20)
project(Rogue_Like_in_Term)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Rogue_Like_in_Term
        Jeu/mainV0.2.cpp
        librairie.hpp
)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(Rogue_Like_in_Term ${CURSES_LIBRARIES})


