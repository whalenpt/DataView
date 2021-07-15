cmake_minimum_required(VERSION 3.18)
project(DataView VERSION 0.0.1 DESCRIPTION "Qt GUI for visualizing simple data files")
option(DATAVIEW_STATIC "Build DataView as static (ON) or shared library (OFF)" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DATAVIEW_STATIC)
    set(BUILD_SHARED_LIBS ON)
    message(STATUS "Building shared DataView library")
else()
    set(BUILD_SHARED_LIBS OFF)
    message(STATUS "Building static DataView library")
endif()

add_subdirectory(dataview)
add_subdirectory(demos)

