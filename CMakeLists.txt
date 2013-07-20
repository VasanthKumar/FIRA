
cmake_minimum_required(VERSION 2.6)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/hari/installed/opencv/OpenCV-2.4.3)

project(main)

FIND_PACKAGE(OpenCV REQUIRED)

#set_source_files_properties(wifi.c PROPERTIES LANGUAGE CXX)
add_library(fira arena.cpp ball.cpp colors.cpp contours.cpp our_bot.cpp wifi.c motion.cpp Ximea.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  fira ${OpenCV_LIBS} m3api serial  )

