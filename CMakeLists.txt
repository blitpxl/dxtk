cmake_minimum_required(VERSION 3.10)
project(demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_CXX_FLAGS_DEBUG "")
add_compile_options("/std:c++20" "/Od" "/Zi" "/DEBUG" "/EHsc" "/DUNICODE" "/D_UNICODE"
	#"/fsanitize=address"
)

add_library(dxtk STATIC
	dxtk/button.cpp
	dxtk/control.cpp
	dxtk/column.cpp
	dxtk/dxobject.cpp
	dxtk/label.cpp
	dxtk/mathutils.cpp
	dxtk/inputarea.cpp
	dxtk/rectangle.cpp
	dxtk/row.cpp
	dxtk/slider.cpp
	dxtk/textfield.cpp
	dxtk/timer.cpp
	dxtk/window.cpp
)

set(CMAKE_EXE_LINKER_FLAGS "/SUBSYSTEM:WINDOWS /DEBUG")

add_executable(demo main.cpp)
target_include_directories(demo PRIVATE ./dxtk 
	#./bass
)
target_link_libraries(demo dxtk user32 dwmapi d2d1 dwrite 
	#../bass/bass
)