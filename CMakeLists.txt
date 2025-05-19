cmake_minimum_required(VERSION 3.10)
project(llama-translater)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX /permissive-)
else()
    add_compile_options(-Wall -Werror -Wpedantic)
endif()


include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

set(SOURCES
    src/main.cpp
    src/llhttp.cpp
)

add_executable(llama-translater ${SOURCES})

target_link_libraries(llama-translater libcurl)


add_custom_command(TARGET llama-translater POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/bin/libcurl.dll
        $<TARGET_FILE_DIR:llama-translater>)
