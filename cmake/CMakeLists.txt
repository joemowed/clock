cmake_minimum_required(VERSION 3.28)

# Include toolchain file
include("arm-none-eabi.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#noeabi should mostly support this
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#use the gnu standard
set(CMAKE_CXX_EXTENSIONS ON)

#add_compile_definitions(__SAMD51J20A__ __ATSAMD51J20A__ __GNUC__)

include_guard()

# Define the build type, DEBUG by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "DEBUG")
endif()

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    # Enable compile command to ease indexing with e.g. clangd
    set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
endif()
if(CMAKE_BUILD_TYPE MATCHES RELEASE)
    # Disable compile command to ease indexing with e.g. clangd
    set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
endif()
project(clock VERSION 1.0
    DESCRIPTION ""
    LANGUAGES C CXX)



message("Build type: " ${CMAKE_BUILD_TYPE})



# Enable CMake support for ASM and C languages
enable_language(CXX ASM)


set(CMSIS_INCLUDE_PATH ${CMAKE_SOURCE_DIR}/../samd51/CMSIS)
set(DFP_INLCUDE_PATH ${CMAKE_SOURCE_DIR}/../samd51/DFP)
set(USER_SRC_PATH ${CMAKE_SOURCE_DIR}/../src)
# Add sources to executable
FILE(GLOB SOURCES "../src/*.cpp")
file(GLOB STARTUP_SOURCES "${CMAKE_SOURCE_DIR}/../samd51/DFP/src/*.cpp")

include_directories(${CMSIS_INCLUDE_PATH} ${DFP_INLCUDE_PATH} ${USER_SRC_PATH}/include)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
${SOURCES}
${STARTUP_SOURCES}
)

