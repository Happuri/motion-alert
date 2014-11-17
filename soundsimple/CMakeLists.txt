cmake_minimum_required(VERSION 2.8.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(Boost 1.49 REQUIRED COMPONENTS system filesystem iostreams)
set(Boost_LIBRARIES "${Boost_LIBRARIES}")
find_library(SFML REQUIRED COMPONENTS system window audio)
find_library(FFTW3_LIBRARIES NAMES fftw3 libfftw3)

include_directories(src/otshell_utils)
add_subdirectory(src)

