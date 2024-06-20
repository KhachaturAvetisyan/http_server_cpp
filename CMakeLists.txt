cmake_minimum_required(VERSION 3.20)
project(http_server)

# init options flags
option(SERVER_TESTS "build tests (or no)" OFF)
option(CONSOLE_LOGGER "enable console logger (or no)" OFF)
option(SYSTEM_LOGGER "enable system logger (or no)" OFF)
option(NON_BLOCKING "enable non-blocking sockets (or no)" OFF)

# check Debug or Release build
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug build")
    add_definitions(-D DEBUG_FLAG)
    add_definitions(-D SPDLOG_ACTIVE_LEVEL=1)
else()
    message("Release build")
endif()

# check options flag
message("CONSOLE_LOGGER: ${CONSOLE_LOGGER}")
if (CONSOLE_LOGGER)
    message("CONSOLE_LOGGER enabled!!")
    add_definitions(-D CONSOLE_LOGGER)
endif()

message("SYSTEM_LOGGER: ${SYSTEM_LOGGER}")
if (SYSTEM_LOGGER)
    message("SYSTEM_LOGGER enabled!!")
    add_definitions(-D SYSTEM_LOGGER)
endif()

# check NON_BLOCKING flag and add definition
message("NON_BLOCKING: ${NON_BLOCKING}")
if (NON_BLOCKING)
    message("NON_BLOCKING enabled!!")
    add_definitions(-D NON_BLOCKING)
endif()

# set compiler flags
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")

# set build options
add_compile_options(
        -pipe
        -pie
        -fpie
        -fexceptions
        -fno-strict-overflow
        -fwrapv
        -static-libgcc
        -fstack-protector-strong
        -fms-extensions
        -Wformat
        -Wformat-security
        -Wno-missing-field-initializers
        -Wno-unused-function
        -Wno-unused-but-set-variable
        -Wall
        -Wextra
        -Wpedantic
)


# set include directories
include_directories(
        "${PROJECT_SOURCE_DIR}/headers"
        "${PROJECT_SOURCE_DIR}/headers/interfaces/"
        "${PROJECT_SOURCE_DIR}/tpp"
)

# set source files
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/headers/*.hpp"
        "${PROJECT_SOURCE_DIR}/headers/interfaces/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/tpp/*.tpp"
)

# add executable files
add_executable(${PROJECT_NAME} main.cpp)

add_library(proj_src  ${all_SRCS})
target_compile_features(proj_src PUBLIC cxx_std_23)


# include 3rdParties libs
# find 3rdParties libs
find_package(Pistache)
find_package(spdlog)
find_package(libconfig)

# link 3rdParties libs include directories
include_directories(
        # 3rdParties libs include directories
        ${spdlog_INCLUDE_DIRS}
        ${fmt_INCLUDE_DIRS}
        ${libconfig_INCLUDE_DIRS}
        ${Pistache_INCLUDE_DIR}
)

# link 3rdParties libs
target_link_libraries(
        ${PROJECT_NAME} PRIVATE

        proj_src

        # 3rdParties libs link
        Pistache::Pistache
        spdlog::spdlog
        libconfig::libconfig
)

add_compile_definitions(SPDLOG_FMT_EXTERNAL)

# enable tests and on/off
message("SERVER_TESTS: ${SERVER_TESTS}")

# ON or OFF UNIT TESTS
if (SERVER_TESTS)
    message("Unit tests enabled!!")
    enable_testing()
    add_subdirectory(tests)
endif()