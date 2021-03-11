cmake_minimum_required(VERSION 3.0.0)

if(NOT DEFINED VCPKG_ROOT)
	if(DEFINED ENV{VCPKG_ROOT})
		set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE PATH "Path to vcpkg directory")
	endif()
endif()

if(DEFINED VCPKG_ROOT)
	set(
		CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
		CACHE PATH "Path to vcpkg toolchain")

	if(NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
		message(FATAL_ERROR "Invalid VCPKG_ROOT specified (${VCPKG_ROOT}); please specify an absolute path")
    endif()
else()
    message(FATAL_ERROR "Need VCPKG_ROOT defined")
endif()

project(mem_file VERSION 0.1.0)

message("CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")

find_package(ZLIB REQUIRED)
find_package(GTest CONFIG REQUIRED)
include(CTest)
enable_testing()

add_library(mem_file mem_file.cpp mem_file.h base64.cpp base64.h)
target_link_libraries(mem_file PRIVATE ZLIB::ZLIB)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE mem_file)

add_executable(unit_test unit_test.cpp)
add_test(NAME unit_test COMMAND unit_test)
target_link_libraries(unit_test PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main mem_file)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
