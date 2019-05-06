cmake_minimum_required(VERSION 3.9)
project(downloader)

add_executable(downloader_libcurl_test
        main.cpp
        ../downloader/downloader-libcurl.h)

target_link_libraries(downloader_libcurl_test
        downloader)