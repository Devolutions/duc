
set(MODULE_NAME "duc-core")
set(MODULE_PREFIX "LIBDUC_CORE")

set(${MODULE_PREFIX}_HEADERS "")

set(${MODULE_PREFIX}_SOURCES
	DucPixelFormat.c
	DucPixelBuffer.c
	#DucTape.c
	Duc.c)

add_library(${MODULE_NAME} STATIC ${${MODULE_PREFIX}_SOURCES} ${${MODULE_PREFIX}_HEADERS})

target_include_directories(${MODULE_NAME}
	INTERFACE $<INSTALL_INTERFACE:include>)

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})

if(BUILD_TESTING)
	add_subdirectory(test)
endif()

if(DUC_INSTALL)
	install(TARGETS ${MODULE_NAME}
		EXPORT duc-targets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib)
endif()
