cmake_minimum_required(VERSION 3.12)
message(STATUS "CMake version: ${CMAKE_VERSION}")

project(webCam2RTSP
  VERSION
    1.0
  DESCRIPTION
    "Web cam access via RTSP Server"
  LANGUAGES
    CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Network)
if(Qt5_FOUND)
    message(STATUS "${PROJECT_NAME} Found Qt5 version: ${Qt5_VERSION}")
else()
    message(FATAL_ERROR "Qt5 not Found !!")
endif()


# Find GStreamer RTSP Server and its dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST_RTSP REQUIRED gstreamer-rtsp-server-1.0)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

include_directories(
  ${GST_INCLUDE_DIRS}
  ${GST_RTSP_INCLUDE_DIRS}
)

link_directories(
  ${GST_LIBRARY_DIRS}
  ${GST_RTSP_LIBRARY_DIRS}
)


set(SRC_FILES
                main.cpp
                )

add_executable(${PROJECT_NAME} ${SRC_FILES})

add_definitions(-DQT_NO_VERSION_TAGGING)

target_link_libraries(${PROJECT_NAME}
  ${GST_LIBRARIES}
  ${GST_RTSP_LIBRARIES}
  Qt5::Network
  Qt5::Core
)

include_directories(.)
