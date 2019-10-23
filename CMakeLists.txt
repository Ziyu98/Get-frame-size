cmake_minimum_required(VERSION 3.10)
project(h264Analyzer CXX)
set(CMAKE_C_COMPILER "/home/mdl/huz123/software/install/bin/gcc")
set(CMAKE_CXX_COMPILER "/home/mdl/huz123/software/install/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/home/mdl/huz123/software/boost_1_64_0 -L/home/mdl/huz123/software/boost_1_64_0/stage/lib  -lboost_system  -lboost_filesystem -lboost_iostreams -O2 -g")

set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cc main.hh NALUAnalyzer.cc NALUAnalyzer.hh bitAnalzyer.cc bitAnalzyer.hh x264_smaple.hh nalu_test.cc nalu_test.hh)