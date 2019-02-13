cmake_minimum_required(VERSION 3.8)

project (photo_misc LANGUAGES CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set (source_dir "${PROJECT_SOURCE_DIR}/source/")

target_compile_features(photo_misc PUBLIC
cxx_std_17)

file (GLOB source_files "${source_dir}/*.cpp")

add_executable (main ${source_files})

