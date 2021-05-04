cmake_minimum_required(VERSION 3.19.1)
set(CMAKE_CXX_STANDARD 17)

project(pwutils VERSION 0.0.1 LANGUAGES CXX)

set(SRC_FILES
	src/pwstats.cpp
	src/pwthreads.cpp
	src/pwdataio.cpp
	src/pwdir.cpp
	src/pwstrings.cpp
	src/pwmath.cpp
)

set(INCLUDE_FILES
	include/pwutils/pwstats.h
	include/pwutils/pwthreads.h
	include/pwutils/pwexcept.h
	include/pwutils/pwdataio.hpp
	include/pwutils/pwconstants.h
	include/pwutils/pwdir.h
	include/pwutils/pwstrings.h
	include/pwutils/pwmath.h
)

add_library(pwutils SHARED ${SRC_FILES} ${INCLUDE_FILES})
add_library(pw::pwutils ALIAS pwutils)

target_include_directories(pwutils PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/pwutils>
	$<INSTALL_INTERFACE:include/pwutils>
)

add_library(pwutils_static STATIC ${SRC_FILES} ${INCLUDE_FILES})
add_library(pw::pwutils_static ALIAS pwutils_static)

target_include_directories(pwutils_static PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/pwutils>
	$<INSTALL_INTERFACE:include/pwutils>
)

include(GNUInstallDirs)
set_target_properties(pwutils pwutils_static PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 0
)

install(TARGETS pwutils pwutils_static
	EXPORT PWUtilsTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} 
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${INCLUDE_FILES}
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/pwutils"
)

install(EXPORT PWUtilsTargets
	FILE PWUtilsTargets.cmake
	NAMESPACE pw::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pwutils
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/PWUtilsConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/cmake/PWUtilsConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pwutils
	PATH_VARS CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/cmake/PWUtilsConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/PWUtilsConfig.cmake
				${CMAKE_CURRENT_BINARY_DIR}/cmake/PWUtilsConfigVersion.cmake
				DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pwutils
)





