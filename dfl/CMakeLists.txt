project(dfl)

file(GLOB_RECURSE SOURCES *.cxx)
file(GLOB_RECURSE HEADERS *.h)

add_library(dfl STATIC ${SOURCES} ${HEADERS})
target_include_directories(dfl PUBLIC include)
target_include_directories(dfl PRIVATE include/dfl)
target_compile_features(dfl PUBLIC cxx_std_23)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(dfl PRIVATE -D_DEBUG)
endif ()
