cmake_minimum_required(VERSION 3.13...3.19)
project(ha6gui LANGUAGES C CXX)


set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
set (EXE "ha6gui" )

include(glad)
include(imgui)

add_executable(${EXE} WIN32
	main.cpp
	res/res.rc
)

target_include_directories(${EXE} PRIVATE ".")
target_compile_definitions(${EXE} PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN)

find_package(OpenGL REQUIRED)
target_link_libraries(${EXE} PRIVATE imgui glad OpenGL::GL)