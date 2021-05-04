cmake_minimum_required(VERSION 3.5)

set(THIS Mathfun)
project(${THIS} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(Mathfun
    main.cpp
    mainwindow.cpp
    mainwindow.h
)
target_include_directories(${THIS}
    PUBLIC ${CMAKE_SOURCE_DIR}/dataview
)

target_link_libraries(${THIS}
    PRIVATE Qt6::Widgets
    PRIVATE dataview
)
