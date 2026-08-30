cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER g++)

project(osis2json CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

file(GLOB_RECURSE OSIS2Json_Source 
        "src/*.hpp"
        "src/*.cpp"
)


# OSIS2Json project options
add_executable(osis2json ${OSIS2Json_Source})

target_include_directories(osis2json PUBLIC "src/include/thirdparty")
target_include_directories(osis2json PUBLIC "src/include")

target_compile_options(osis2json PRIVATE -O0)
