cmake_minimum_required(VERSION 3.5)
project (GLTFSDK)

option(ENABLE_UNIT_TESTS "ENABLE_UNIT_TESTS" ON)
if(ENABLE_UNIT_TESTS)
    enable_testing()
endif()

# Disable the samples on macOS, iOS, and Android since the experimental features they use
# do not yet build with XCode or clang on these platforms.
if(APPLE OR ANDROID_OS_PLATFORM)
    option(ENABLE_SAMPLES "ENABLE_SAMPLES" OFF)
else()
    option(ENABLE_SAMPLES "ENABLE_SAMPLES" ON)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -D_DEBUG -DFEATURE_ASSERTS_ENABLED")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11)

if (WIN32)
    # Define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING to disable the warnings in the current version of Google Test (1.8.0)
    # TODO: Newer versions shouldn't have this problem.  Re-evaluate this when upgrading.
    add_compile_definitions(
        _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
    )
    # Multithreaded Build
    # add_compile_options(/MP)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Build/CMake/Modules")

find_package(PkgConfig REQUIRED)
pkg_check_modules(nlohmann_json REQUIRED IMPORTED_TARGET nlohmann_json)
find_package(GTest CONFIG)
if(NOT GTest_FOUND)
    # import and alias just like it is already installed via VcPkg
    add_subdirectory(External/googletest)
    add_library(GTest::gtest_main ALIAS gtest_main)
endif()

add_subdirectory(GLTFSDK)

target_link_libraries(GLTFSDK PRIVATE ${nlohmann_json_LIBRARIES})

if(ENABLE_UNIT_TESTS AND (NOT DEFINED EMSCRIPTEN))
    add_subdirectory(GLTFSDK.TestUtils)
    add_subdirectory(GLTFSDK.Test)
endif()

if(ENABLE_SAMPLES AND (NOT DEFINED EMSCRIPTEN))
    add_subdirectory(GLTFSDK.Samples)
endif()
