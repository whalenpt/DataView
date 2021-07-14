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

# Build ParamBin and pwutils libraries statically
set(PARAMBIN_STATIC ON CACHE BOOL "Build ParamBin as static (ON) or shared library (OFF)")
set(PWUTILS_STATIC ON CACHE BOOL "Build pwutils as static (ON) or shared library (OFF)")
add_subdirectory(external/ParamBin)
add_subdirectory(external/pwutils)

set(SOURCE_LIST
	dataaux.cpp
	dataview.cpp
	draglistview.cpp
	dropchartview.cpp
	fileaux.cpp
	graphframe.cpp
	threecolstacked.cpp
	twocol.cpp
	twocol_log.cpp
	twocol_m.cpp
	twocol_m_log.cpp
)

set(HEADER_LIST
	dataaux.h
	dataview.h
	dataview_global.h
	draglistview.h
	dropchartview.h
	fileaux.h
	graphframe.h
	threecolstacked.h
	twocol.h
	twocol_log.h
	twocol_m.h
	twocol_m_log.h
)

add_library(dataview SHARED ${SOURCE_LIST} ${HEADER_LIST})
target_link_libraries(dataview
	PRIVATE Qt6::Core
	PRIVATE Qt6::Gui
	PRIVATE Qt6::Widgets
	PRIVATE Qt6::Charts
	PUBLIC ParamBin::parambin
	PUBLIC pw::pwutils
)

#target_include_directories(dataview 
#	PUBLIC external/ParamBin/include
#	PUBLIC external/pwutils/include
#)

target_compile_definitions(dataview PRIVATE DATAVIEW_LIBRARY)


