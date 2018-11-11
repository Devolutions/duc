
add_subdirectory(core)
add_subdirectory(tape)
add_subdirectory(utils)

if(WITH_WAYK)
	include(WaykSDK)
endif()

if(NOT DEFINED WITH_SIMD)
	set(WITH_SIMD 1)
endif()

if(NOT DEFINED WITH_HALIDE AND WAYK_LLVM_FOUND)
	set(WITH_HALIDE 1)
endif()

add_subdirectory(xpp)

