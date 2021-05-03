cmake_minimum_required(VERSION 3.19)
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS QtCharts REQUIRED)

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
	PRIVATE Qt6::QtCharts
)
target_compile_definitions(dataview PRIVATE DATAVIEW_LIBRARY)






