
set(MODULE_NAME "duc-utils")
set(MODULE_PREFIX "LIBDUC_UTILS")

set(${MODULE_PREFIX}_HEADERS "")

set(${MODULE_PREFIX}_SOURCES
	DucTime.c)

add_library(${MODULE_NAME} STATIC ${${MODULE_PREFIX}_SOURCES} ${${MODULE_PREFIX}_HEADERS})

target_include_directories(${MODULE_NAME}
	INTERFACE $<INSTALL_INTERFACE:include>)

set(${MODULE_PREFIX}_LIBS duc-core)

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})

if(DUC_INSTALL)
	install(TARGETS ${MODULE_NAME}
		EXPORT duc-targets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib)
endif()

