cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(check_confidence_pj LANGUAGES CXX)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

link_directories(${OpenCV_LIBRARY_DIRS})

add_executable(check_face_confidence check_face_confidence.cpp)

target_link_libraries(check_face_confidence ${OpenCV_LIBS})

add_executable(check_eye_confidence check_eye_confidence.cpp)

target_link_libraries(check_eye_confidence ${OpenCV_LIBS})
