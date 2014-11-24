cmake_minimum_required(VERSION 2.8.6)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(Boost 1.49 REQUIRED COMPONENTS system filesystem iostreams)
set(Boost_LIBRARIES "${Boost_LIBRARIES}")
find_library(SFML REQUIRED COMPONENTS system window audio)
find_library(FFTW3_LIBRARIES NAMES fftw3 libfftw3)

option(BUILD_TESTS "Build the unit tests." OFF)



if(BUILD_TESTS)
  include(CTest)
  enable_testing()

  set(GTEST_ROOT "dep/gtest")
  set(GTEST_FOUND ON)
  set(GTEST_INCLUDE_DIRS ${GTEST_ROOT}/include)
  set(GTEST_LIBRARIES gtest)
  set(GTEST_MAIN_LIBRARIES gtest_main)
  set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
endif()


execute_process(COMMAND git "submodule" "update" "--init" "--recursive")


add_subdirectory(src)

if(BUILD_TESTS)
	add_subdirectory(deps)
	add_subdirectory(tests)
endif()
