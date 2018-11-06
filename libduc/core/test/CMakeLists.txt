
set(MODULE_NAME "TestDucCore")
set(MODULE_PREFIX "TEST_DUC_CORE")

add_executable(${MODULE_NAME} TestDucCore.c)

target_link_libraries(${MODULE_NAME} duc-core munit)

set_target_properties(${MODULE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TESTING_OUTPUT_DIRECTORY}")

add_test(${MODULE_NAME} ${TESTING_OUTPUT_DIRECTORY}/${MODULE_NAME})

