cmake_minimum_required(VERSION 2.8)
project(Gobang)
add_library(Gobang SHARED
	Gobang.cpp
	Gobang_API.cpp
	SCalculate.cpp
	ZMap.cpp
)
add_definitions(-D GOBANG_EXPORTS)