cmake_minimum_required (VERSION 3.5)

project(Project5)
set (CMAKE_CXX_STANDARD 11)
SET(MACOSX_DEPLOYMENT_TARGET ${DARWIN_MAJOR_SDK_VERSION}.${DARWIN_MINOR_SDK_VERSION})
#set(CMAKE_CXX_FLAGS "--sysroot ${CMAKE_OSX_SYSROOT} ${CMAKE_CXX_FLAGS}")
MESSAGE("Setting MACOSX_DEPLOYMENT_TARGET to '${MACOSX_DEPLOYMENT_TARGET}'.")
#SET(CMAKE_OSX_DEPLOYMENT_TARGET "")
#SET(CMAKE_OSX_SYSROOT /)
include_directories("${CMAKE_SOURCE_DIR}")
set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
find_library(TWIT_CURL_LIBRARY twitcurl ${CMAKE_SOURCE_DIR}/libraries)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/bsoncxx/v_noabi")

find_package( Boost  COMPONENTS
        program_options
        system
        filesystem
        thread REQUIRED )

find_package(openssl COMPONENTS crypto ssl REQUIRED)
include_directories( ${Boost_INCLUDE_DIR} )
find_package(cpprestsdk REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

link_libraries(${JSONCPP_LIBRARIES})
add_executable(Project5
        main.cpp)

target_link_libraries(Project5
        ${Boost_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${TWIT_CURL_LIBRARY}
        ${JSONCPP_LIBRARIES}
        ${LIBMONGOCXX_LIBRARIES}
        ${LIBBSONCXX_LIBRARIES}
        cpprestsdk::cpprest
        )