
set(MUNIT_SOURCES munit.c)
set(MUNIT_HEADERS munit.h)

add_library(munit STATIC ${MUNIT_SOURCES} ${MUNIT_HEADERS})

