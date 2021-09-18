cmake_minimum_required(VERSION 3.18)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 6.1.2 COMPONENTS Charts REQUIRED)
find_package(Qt6 6.1.2 COMPONENTS DataVisualization REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


set(SOURCE_LIST
	dataview.cpp
	draglistview.cpp
	core/dataaux.cpp
	core/dataaux3D.cpp
	graphframe/graphframe.cpp
	graphframe/dropchartview.cpp
	graphframe/axesaux.cpp
	graphframe/threecolstacked.cpp
	graphframe/twocol.cpp
	graphframe/surfacewidget.cpp
)

set(INCL_DIR "${DataView_SOURCE_DIR}/include")
set(HEADER_LIST
	${INCL_DIR}/dataview.h
	${INCL_DIR}/dataview_global.h
	${INCL_DIR}/draglistview.h
	${INCL_DIR}/core/dataaux.h
	${INCL_DIR}/core/dataaux3D.h
	${INCL_DIR}/graphframe/graphframe.h
	${INCL_DIR}/graphframe/dropchartview.h
    ${INCL_DIR}/graphframe/axesaux.h
	${INCL_DIR}/graphframe/threecolstacked.h
	${INCL_DIR}/graphframe/twocol.h
	${INCL_DIR}/graphframe/surfacewidget.h
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
	PUBLIC Qt6::Core
	PUBLIC Qt6::Gui
	PUBLIC Qt6::Widgets
	PUBLIC Qt6::Charts
    PUBLIC Qt6::DataVisualization
	PUBLIC ParamBin::parambin
	PUBLIC pw::pwutils
)

target_compile_definitions(dataview PRIVATE DATAVIEW_LIBRARY)


