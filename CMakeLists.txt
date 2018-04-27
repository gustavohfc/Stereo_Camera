cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")

project( Projeto1 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

#add_library(common common.cpp common.h)

add_executable( Requisito1 Requisito1.cpp )

target_link_libraries( Requisito1 ${OpenCV_LIBS} )
#target_link_libraries( Requisito1 ${OpenCV_LIBS} common )
