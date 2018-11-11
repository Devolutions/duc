
set(MODULE_NAME "duc-xpp")
set(MODULE_PREFIX "DUC_XPP")

include_directories(include)

set(${MODULE_PREFIX}_HEADERS "")

set(${MODULE_PREFIX}_SOURCES
	XppCodec.c
	XppColor.c
	XppScale.c
	XppPrim.c)

if(WITH_HALIDE)
	include(WaykSDK.cmake)
	add_subdirectory(halide)
	list(APPEND ${MODULE_PREFIX}_HEADERS ${HALIDE_GENERATED_HEADERS})
	list(APPEND ${MODULE_PREFIX}_SOURCES ${HALIDE_GENERATED_OBJECTS})
	set_source_files_properties(${HALIDE_GENERATED_HEADERS} PROPERTIES GENERATED TRUE)
	set_source_files_properties(${HALIDE_GENERATED_OBJECTS} PROPERTIES GENERATED TRUE)
endif()

if(NOT DEFINED WITH_CODEC_SIMD)
	set(WITH_CODEC_SIMD 1)
endif()

if(WITH_CODEC_SIMD)
	add_definitions(-DWITH_CODEC_SIMD)

	list(APPEND ${MODULE_PREFIX}_SIMD_SOURCES
		simd/simd.c)
	list(APPEND ${MODULE_PREFIX}_SIMD_HEADERS
		simd/simd.h)

	if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
		set_source_files_properties(${${MODULE_PREFIX}_SIMD_SOURCES} PROPERTIES COMPILE_FLAGS "-mfpu=neon")
	else()
		if(MSVC)
			set_source_files_properties(${${MODULE_PREFIX}_SIMD_SOURCES} PROPERTIES COMPILE_FLAGS "/arch:SSE2")
		else()
			set_source_files_properties(${${MODULE_PREFIX}_SIMD_SOURCES} PROPERTIES COMPILE_FLAGS "-msse2" )
		endif()
	endif()

	list(APPEND ${MODULE_PREFIX}_SOURCES ${${MODULE_PREFIX}_SIMD_SOURCES})
	list(APPEND ${MODULE_PREFIX}_HEADERS ${${MODULE_PREFIX}_SIMD_HEADERS})
endif()

if(JPEG_FOUND)
	include_directories(${JPEG_INCLUDE_DIR})
endif()

if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIR})
endif()

if(LIBYUV_FOUND)
	include_directories(${LIBYUV_INCLUDE_DIR})
endif()

add_library(${MODULE_NAME} STATIC ${${MODULE_PREFIX}_SOURCES} ${${MODULE_PREFIX}_HEADERS})

list(APPEND ${MODULE_PREFIX}_LIBS wayk-utils)
list(APPEND ${MODULE_PREFIX}_LIBS winpr)

if(WITH_HALIDE)
	add_dependencies(${MODULE_NAME} halide-generate-all)
endif()

if(JPEG_FOUND)
	list(APPEND ${MODULE_PREFIX}_LIBS jpeg-turbo)
endif()

if(PNG_FOUND)
	list(APPEND ${MODULE_PREFIX}_LIBS png)
endif()

if(LIBYUV_FOUND)
	list(APPEND ${MODULE_PREFIX}_LIBS libyuv)
endif()

target_include_directories(${MODULE_NAME}
	INTERFACE $<INSTALL_INTERFACE:include>)

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})

if(WAYK_INSTALL)
	install(TARGETS ${MODULE_NAME}
		EXPORT duc-targets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib)
endif()
