
set(MODULE_NAME "lz4")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(LZ4_SOURCES lz4.c)
set(LZ4_HEADERS lz4.h)

set(LZ4_HC_SOURCES lz4hc.c)
set(LZ4_HC_HEADERS lz4hc.h lz4opt.h)

add_library(lz4 STATIC
	${LZ4_SOURCES} ${LZ4_HEADERS}
	${LZ4_HC_SOURCES} ${LZ4_HC_HEADERS})

if(DUC_INSTALL)
	install(TARGETS ${MODULE_NAME}
		EXPORT duc-targets
		ARCHIVE DESTINATION lib
		LIBRARY DESTINATION lib)
endif()

