cmake_minimum_required(VERSION 3.18)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 6.1.0 COMPONENTS Charts REQUIRED)


set(SOURCE_LIST
	dataview.cpp
	core/dataaux.cpp
	core/fileaux.cpp
	listgui/draglistview.cpp
	graphgui/axesaux.cpp
	graphgui/dropchartview.cpp
	graphgui/graphframe.cpp
	graphgui/graphtype/threecolstacked.cpp
	graphgui/graphtype/twocol.cpp
	graphgui/graphtype/twocol_m.cpp
)

set(INCL_DIR "${DataView_SOURCE_DIR}/include")
set(HEADER_LIST
	${INCL_DIR}/dataview.h
	${INCL_DIR}/dataview_global.h
	${INCL_DIR}/core/dataaux.h
	${INCL_DIR}/core/fileaux.h
	${INCL_DIR}/listgui/draglistview.h
    ${INCL_DIR}/graphgui/axesaux.h
	${INCL_DIR}/graphgui/dropchartview.h
	${INCL_DIR}/graphgui/graphframe.h
	${INCL_DIR}/graphgui/graphtype/threecolstacked.h
	${INCL_DIR}/graphgui/graphtype/twocol.h
	${INCL_DIR}/graphgui/graphtype/twocol_m.h
)

add_library(dataview ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(dataview PRIVATE
    $<BUILD_INTERFACE:${DataView_SOURCE_DIR}/include/DataView>
    $<INSTALL_INTERFACE:include/DataView>
)
target_include_directories(dataview PUBLIC
    $<BUILD_INTERFACE:${DataView_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(dataview
	PRIVATE Qt6::Core
	PRIVATE Qt6::Gui
	PRIVATE Qt6::Widgets
	PRIVATE Qt6::Charts
	PUBLIC ParamBin::parambin
	PUBLIC pw::pwutils
)

target_compile_definitions(dataview PRIVATE DATAVIEW_LIBRARY)


