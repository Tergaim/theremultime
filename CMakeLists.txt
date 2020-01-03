CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(OpenCV_STATIC OFF)
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

PROJECT(Theremultime)

file(GLOB SOUNDCPP "sound/*.cpp")
file(GLOB SOUNDHPP "sound/*.hpp")
ADD_EXECUTABLE(pgm main.cpp camerafront/camerafront.cpp camerafront/camerafront.hpp ${SOUNDCPP} ${SOUNDHPP} )
ADD_DEFINITIONS( -D__LINUX_ALSA__ -D__LITTLE_ENDIAN__)
TARGET_LINK_LIBRARIES(pgm ${OpenCV_LIBS} -pthread -lasound)