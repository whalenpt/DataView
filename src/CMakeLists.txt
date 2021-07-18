cmake_minimum_required(VERSION 3.18)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 6.1.0 COMPONENTS Charts REQUIRED)


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

set(SOURCE_LIST
	axesaux.cpp
	dataaux.cpp
	dataview.cpp
	draglistview.cpp
	dropchartview.cpp
	fileaux.cpp
	graphframe.cpp
	threecolstacked.cpp
	twocol.cpp
	twocol_m.cpp
)

set(HEADER_LIST
	axesaux.h
	dataaux.h
	dataview.h
	dataview_global.h
	draglistview.h
	dropchartview.h
	fileaux.h
	graphframe.h
	threecolstacked.h
	twocol.h
	twocol_m.h
)

add_library(dataview ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(dataview
	PRIVATE Qt6::Core
	PRIVATE Qt6::Gui
	PRIVATE Qt6::Widgets
	PRIVATE Qt6::Charts
	PUBLIC ParamBin::parambin
	PUBLIC pw::pwutils
)

target_compile_definitions(dataview PRIVATE DATAVIEW_LIBRARY)


