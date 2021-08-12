cmake_minimum_required(VERSION 3.5)

set(THIS optics)
project(${THIS} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${THIS}
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(${THIS}
    PUBLIC Qt6::Widgets
    PUBLIC dataview
)

#message("Current source directory: ${CMAKE_CURRENT_SOURCE_DIR}")

target_include_directories(${THIS}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(copy_assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	${CMAKE_CURRENT_SOURCE_DIR}/data 
	${CMAKE_CURRENT_BINARY_DIR}/data
)

add_dependencies(${THIS} copy_assets)





