cmake_minimum_required(VERSION 3.20)
project(http_server)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME "http_server")

# set build type
set(CMAKE_BUILD_TYPE Release)

# add options
option(DEBUG "Debug mode" OFF)

# set debug mode
if(DEBUG)
    add_definitions(-D DEBUG_VAR)
    message(STATUS "Debug mode")
else()
    message(STATUS "Release mode")
endif()

# set include dir
include_directories(${PROJECT_SOURCE_DIR}/headers)

# set all files
file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/headers/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
    )

# execut source files
add_executable(${PROJECT_NAME} ${all_SRCS})


# install conan libs
execute_process(COMMAND conan install ${PROJECT_SOURCE_DIR}/conanfile.txt --output-folder=${PROJECT_BINARY_DIR} --build=missing)
include(${PROJECT_BINARY_DIR}/conan_toolchain.cmake)


# find 3rdParties libs
#find_package(nlohmann_json REQUIRED)
#find_package(CURL REQUIRED)
find_package(Pistache REQUIRED)

# include 3rdParties libs
#message(STATUS "nlohmann_json_INCLUDE_DIR: ${nlohmann_json_INCLUDE_DIR}")
#message(STATUS "CURL_INCLUDE_DIR: ${CURL_INCLUDE_DIR}")
message(STATUS "Pistache_INCLUDE_DIR: ${Pistache_INCLUDE_DIR}")

include_directories(
#                    ${nlohmann_json_INCLUDE_DIR}
#                    ${CURL_INCLUDE_DIR}
                    ${Pistache_INCLUDE_DIR}
                    )

# link 3rdParties libs
#message(STATUS "nlohmann_json_LIBRARIES: ${nlohmann_json_LIBRARIES}")
#message(STATUS "CURL_LIBRARIES: ${CURL_LIBRARIES}")
message(STATUS "Pistache_LIBRARIES: ${Pistache_LIBRARIES}")

target_link_libraries(
        ${PROJECT_NAME} PUBLIC

#        CURL::libcurl
        Pistache::Pistache
#        nlohmann_json::nlohmann_json
)
