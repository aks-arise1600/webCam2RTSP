cmake_minimum_required(VERSION 3.5)

project(webCam2RTSP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

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
