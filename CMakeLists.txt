cmake_minimum_required(VERSION 3.8)

project (photo_misc LANGUAGES CXX)

add_executable (main_he
                source/basis.cpp
                source/he/main.cpp)

add_executable (main_h2
                source/basis.cpp
                source/h2/main.cpp)

add_executable (main_h
                source/basis.cpp
                source/h/main.cpp)

target_compile_features(main_he PUBLIC
                        cxx_std_11)

target_compile_features(main_h2 PUBLIC
                        cxx_std_11) 

target_compile_features(main_h PUBLIC
                        cxx_std_11)                         

target_include_directories(main_he PUBLIC source)

target_include_directories(main_h2 PUBLIC source)

target_include_directories(main_h PUBLIC source)
