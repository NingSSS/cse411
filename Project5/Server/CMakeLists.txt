cmake_minimum_required (VERSION 3.5)
if (UNIX)
    add_definitions(-Wno-sign-compare -Wno-enum-compare)
endif(UNIX)
project(Server)
set (CMAKE_CXX_STANDARD 11)
include_directories("${CMAKE_SOURCE_DIR}")
set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
find_package( Boost 1.67 COMPONENTS
        program_options
        system
        filesystem
        thread REQUIRED )

find_package(openssl COMPONENTS crypto ssl REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )
find_package(cpprestsdk REQUIRED)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/bsoncxx/v_noabi")

add_executable(Server
        main.cpp
        )

target_link_libraries(Server
        ${Boost_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${TWIT_CURL_LIBRARY}
        ${JSONCPP_LIBRARIES}
        ${LIBMONGOCXX_LIBRARIES}
        ${LIBBSONCXX_LIBRARIES}
        cpprestsdk::cpprest)