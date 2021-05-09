cmake_minimum_required(VERSION 3.5)

project(vectorizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_OPENCV_INSTALL_DIR ${CMAKE_SOURCE_DIR}/dependencies)
make_directory(${CMAKE_OPENCV_INSTALL_DIR})
make_directory(${CMAKE_BINARY_DIR}/output)

find_package(OpenCV PATHS ${CMAKE_OPENCV_INSTALL_DIR} )

add_executable(vectorizer main.cpp )

if(OpenCV_FOUND)
    message("***OpenCV found***")
else()
    message("***OpenCV not found***")
    include(${CMAKE_SOURCE_DIR}/cmake/ExternalProjectOpenCV.cmake)
endif()


target_link_libraries(vectorizer ${OpenCV_LIBRARIES})

add_custom_command(TARGET vectorizer PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources
                       ${CMAKE_BINARY_DIR}/resources
                       )
