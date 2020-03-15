cmake_minimum_required (VERSION 3.13)

find_package(mongo-c-driver CONFIG REQUIRED)

add_library(mongodb SHARED
        Client.hpp
        Client.cpp
        Util.cpp
        Util.hpp
        Database.cpp
        Database.hpp
        MongoDB.cpp
        MongoDB.hpp
        Collection.cpp
        Collection.hpp
)

target_compile_definitions(mongodb PRIVATE ${MONGOC_STATIC_DEFINITIONS})

target_include_directories(mongodb PRIVATE ${MONGOC_STATIC_INCLUDE_DIRS})

if (${WIN32})
    target_link_options(mongodb PRIVATE "/NODEFAULTLIB:LIBCMT")
    target_link_libraries(mongodb PRIVATE ws2_32 wsock32)
endif()

target_link_libraries(mongodb PRIVATE gmod-module-base)
target_link_libraries(mongodb PRIVATE ${MONGOC_STATIC_LIBRARIES})

set_gmod_suffix_prefix(mongodb)