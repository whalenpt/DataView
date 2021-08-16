cmake_minimum_required(VERSION 3.18)
project(DataView VERSION 0.0.1 DESCRIPTION "Qt GUI for visualizing simple data files")
option(DATAVIEW_STATIC "Build DataView as static (ON) or shared library (OFF)" OFF)
option(DATAVIEW_DEMOS "Build DataView demos" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DATAVIEW_STATIC)
    set(BUILD_SHARED_LIBS ON)
    message(STATUS "Building shared DataView library")
else()
    set(BUILD_SHARED_LIBS OFF)
    message(STATUS "Building static DataView library")
endif()

if(NOT TARGET parambin)
    if(NOT DATAVIEW_STATIC)
        set(PARAMBIN_STATIC OFF CACHE BOOL "Build ParamBin as static (ON) or shared library (OFF)")
    else()
        set(PARAMBIN_STATIC ON CACHE BOOL "Build ParamBin as static (ON) or shared library (OFF)")
    endif()
    add_subdirectory(external/ParamBin)
endif()

if(NOT TARGET pwutils)
    if(NOT DATAVIEW_STATIC)
        set(PWUTILS_STATIC OFF CACHE BOOL "Build pwutils as static (ON) or shared library (OFF)")
    else()
        set(PWUTILS_STATIC ON CACHE BOOL "Build pwutils as static (ON) or shared library (OFF)")
    endif()
    add_subdirectory(external/pwutils)
endif()


add_subdirectory(src)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    if(DATAVIEW_DEMOS)
        add_subdirectory(demos)
    endif()
endif()


