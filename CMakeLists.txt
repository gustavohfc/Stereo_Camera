cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_BUILD_TYPE Debug)

project( Projeto1 )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(common common.cpp common.h)

add_executable( Requisito1 Requisito1.cpp )
add_executable( Requisito2_calibracao Requisito2_calibracao.cpp )
add_executable( Requisito2 Requisito2.cpp )

target_link_libraries( Requisito1 ${OpenCV_LIBS} common )
target_link_libraries( Requisito2_calibracao ${OpenCV_LIBS} common )
target_link_libraries( Requisito2 ${OpenCV_LIBS} common )
