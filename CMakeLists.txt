cmake_minimum_required(VERSION 3.12)
project(08_Redo)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5.1 COMPONENTS network audio graphics window system REQUIRED)

add_executable(08_Redo
    main.cpp
    ArrayInformation.cpp
    Unit.cpp
    Move.cpp
    Menu.cpp
    LoadGame.cpp
    Keyboard.cpp
    GenerateWorld.cpp

)

target_link_libraries(08_Redo sfml-window sfml-graphics sfml-audio sfml-network)