cmake_minimum_required(VERSION 3.22.1)
project(Pacman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O2")

# Look for SFML >= 2.0, with the specific subcomponents listed below
FIND_PACKAGE(SFML 2 COMPONENTS system window graphics)

set(lib
        lib/DesignByContract.h
        lib/helper.cpp
        lib/helper.h lib/Singleton.h
)

ADD_LIBRARY(logic
        logic/Observer.h
        logic/World.cpp
        logic/World.h
        logic/EntityModel.cpp
        logic/EntityModel.h
        logic/Stopwatch.cpp
        logic/Stopwatch.h
        logic/Generator.cpp
        logic/Generator.h
        logic/Score.cpp
        logic/Score.h
        logic/AbstractFactory.h
        ${lib}
)

set(view
        representation/view/Camera.cpp
        representation/view/Camera.h
        representation/view/ViewFactory.cpp
        representation/view/ViewFactory.h
        representation/view/EntityView.cpp
        representation/view/EntityView.h
        representation/view/View.h
        representation/view/View.cpp
)

set(control
        representation/control/Game.cpp
        representation/control/Game.h
        representation/control/States.cpp
        representation/control/States.h
)

set(representation
        ${control}
        ${view}
)

# Add include dirs for SFML
INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})

# Executable
add_executable(apman main.cpp ${representation})

# Link target to SFML libs
TARGET_LINK_LIBRARIES(apman  logic sfml-system sfml-graphics sfml-window)

# Install our executable to the CMAKE_INSTALL_PREFIX/bin
INSTALL(TARGETS apman DESTINATION ${CMAKE_INSTALL_PREFIX})


