cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
project(Network LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++20")
endif ()

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_library(NetworkLib STATIC
        src/constants_storage.h
        src/domain/network.cpp
        src/domain/network.h
)

add_executable(Network src/main.cpp
        src/application/application.h
        src/gui/window.h

        src/application/application.cpp
        src/gui/window.cpp)

include_directories(src)

target_link_libraries(Network PRIVATE NetworkLib sfml-graphics iphlpapi.lib)
target_compile_features(Network PRIVATE cxx_std_17)

if (WIN32)
    add_custom_command(
            TARGET Network
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Network>
            VERBATIM)
endif ()

install(TARGETS Network)

# TESTS
# Catch2


# CTest
enable_testing()

#FetchContent_Declare(
#        Catch2
#        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#        GIT_TAG v3.4.0 # or a later release
#)
#
#FetchContent_MakeAvailable(Catch2)
#add_executable(scheduler_tests tests/scheduler_tests.cpp)
#
#target_link_libraries(scheduler_tests PRIVATE SchedulerLib Catch2::Catch2WithMain)