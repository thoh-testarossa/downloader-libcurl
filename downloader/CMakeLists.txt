cmake_minimum_required(VERSION 3.9)
project(downloader)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

add_library(downloader
        ../include/config.h
        downloader-libcurl.h
        downloader-libcurl.cpp)

target_link_libraries(downloader
        ${CURL_LIBRARIES}
        pthread)
