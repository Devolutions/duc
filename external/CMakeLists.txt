
if(WITH_LZ4)
	add_subdirectory(lz4)
endif()

if(BUILD_TESTING)
	add_subdirectory(munit)
endif()

