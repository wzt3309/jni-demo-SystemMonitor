#
# **ATTENTION PLEASE**
# Recommand to use maven plugin (see mvn.cmd) to compile
#

cmake_minimum_required(VERSION 3.8)
project(sysm_native)

set(CMAKE_C_STANDARD 99)
set(SOURCE_FILES src/main/javasysm.c
        src/main/version.c
        src/main/sysm_format.c
        src/main/sysm_util.c
        src/main/sysm.c
)

include_directories(include/main
        $ENV{JAVA_HOME}/include
        $ENV{JAVA_HOME}/include/linux
)

add_subdirectory(src/test)

add_library(sysm-amd64-linux SHARED ${SOURCE_FILES})