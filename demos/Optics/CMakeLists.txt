cmake_minimum_required(VERSION 3.5)

set(THIS optics)
project(${THIS} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${THIS}
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

add_custom_target(copy_assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory 
	${CMAKE_CURRENT_SOURCE_DIR}/data 
	${CMAKE_CURRENT_BINARY_DIR}/data
)

add_dependencies(${THIS} copy_assets)





