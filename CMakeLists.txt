cmake_minimum_required(VERSION 3.0)

project(duc C)

include(CTest)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(NOT DEFINED DUC_INSTALL)
	set(DUC_INSTALL ON)
endif()

include(CMakePackageConfigHelpers)

file(STRINGS "${CMAKE_SOURCE_DIR}/VERSION" VERSION_STRINGS)
list(GET VERSION_STRINGS 0 VERSION_STRING)

string(REGEX REPLACE "([0-9]+).[0-9]+.[0-9]+" "\\1" DUC_VERSION_MAJOR ${VERSION_STRING})
string(REGEX REPLACE "[0-9]+.([0-9]+).[0-9]+" "\\1" DUC_VERSION_MINOR ${VERSION_STRING})
string(REGEX REPLACE "[0-9]+.[0-9]+.([0-9]+)" "\\1" DUC_VERSION_PATCH ${VERSION_STRING})
set(DUC_VERSION "${DUC_VERSION_MAJOR}.${DUC_VERSION_MINOR}.${DUC_VERSION_PATCH}")

if(MSVC)
	include(MSVCRuntime)

	if(NOT DEFINED MSVC_RUNTIME)
		set(MSVC_RUNTIME "static")
	endif()

	configure_msvc_runtime()
endif()

if(WIN32)
	set(C_FLAGS "")
	set(C_FLAGS "${C_FLAGS} -D_UNICODE")
	set(C_FLAGS "${C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	set(C_FLAGS "${C_FLAGS} -DWIN32_LEAN_AND_MEAN")
	set(C_FLAGS "${C_FLAGS} -D_WINSOCK_DEPRECATED_NO_WARNINGS")
	set(C_FLAGS "${C_FLAGS} -DWINVER=0x0601 -D_WIN32_WINNT=0x0601")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_FLAGS}")
endif()

set(CMAKE_EXPORT_DIR "lib/cmake/duc")

include_directories(include)

set(EXTERNAL_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

if(NOT DEFINED WITH_LZ4)
	set(WITH_LZ4 ON)
	add_definitions(-DWITH_LZ4)
	include_directories("${EXTERNAL_SOURCE_DIR}/lz4")
endif()

add_subdirectory(external)

if(BUILD_TESTING)
	include_directories("${EXTERNAL_SOURCE_DIR}/munit")
endif()

add_subdirectory(libduc)

# installation

if(DUC_INSTALL)
	export(PACKAGE duc)

	configure_package_config_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/duc-config.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/duc-config.cmake"
		INSTALL_DESTINATION "${CMAKE_EXPORT_DIR}")

	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/duc-config-version.cmake"
		VERSION ${DUC_VERSION}
		COMPATIBILITY SameMajorVersion)

	install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/duc"
		DESTINATION include
		FILES_MATCHING PATTERN "*.h")

	install(FILES
		${PROJECT_BINARY_DIR}/duc-config.cmake
		${PROJECT_BINARY_DIR}/duc-config-version.cmake
		DESTINATION "${CMAKE_EXPORT_DIR}")
endif()

