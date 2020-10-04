cmake_minimum_required(VERSION 3.17)
project(transcend)

set(CMAKE_CXX_STANDARD 17)

if ($ENV{RELEASE} MATCHES "true")
    add_definitions(-DRELEASE_MODE)
endif()

add_executable(transcend main.cpp Entity.cpp Entity.h Room.cpp Room.h Door.cpp Door.h Player.cpp Player.h Monster.cpp Monster.h Wall.cpp Wall.h ChestKey.cpp ChestKey.h Fire.cpp Fire.h Chest.cpp Chest.h Amulet.cpp Amulet.h)

target_include_directories(transcend PUBLIC "..\\jengine_win\\src\\include")
target_link_directories(transcend PUBLIC "..\\jengine_win\\src\\lib")

target_link_libraries(transcend jengine.lib glfw3.lib glew32s.lib freetype.lib glm.lib OpenAL32.lib SOIL.lib opengl32.lib)