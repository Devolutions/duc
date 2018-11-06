
set(MODULE_NAME "duc-tape")
set(MODULE_PREFIX "LIBDUC_TAPE")

set(${MODULE_PREFIX}_HEADERS "")

set(${MODULE_PREFIX}_SOURCES
	DucTape.c)

add_library(${MODULE_NAME} STATIC ${${MODULE_PREFIX}_SOURCES} ${${MODULE_PREFIX}_HEADERS})

target_include_directories(${MODULE_NAME}
	INTERFACE $<INSTALL_INTERFACE:include>)

set(${MODULE_PREFIX}_LIBS duc-core duc-utils)

target_link_libraries(${MODULE_NAME} ${${MODULE_PREFIX}_LIBS})

if(DUC_INSTALL)
	install(TARGETS ${MODULE_NAME}
		EXPORT duc-targets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib)
endif()

