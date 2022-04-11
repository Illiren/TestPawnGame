cmake_minimum_required(VERSION 3.5)

project(Test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

file(GLOB SOURCES_DIR source/*.cpp)
file(GLOB INCLUDE_DIR include/*.hpp)

#set(SOURCES "source/*.cpp")
#set(HEADERS include/*.hpp)

include_directories("include/")

file(COPY resources/blackpawn.png DESTINATION "${CMAKE_BINARY_DIR}/resources")
file(COPY resources/whitepawn.png DESTINATION "${CMAKE_BINARY_DIR}/resources")
file(COPY resources/xkcd-script.ttf DESTINATION "${CMAKE_BINARY_DIR}/resources")
add_executable(Test main.cpp ${SOURCES_DIR} ${INCLUDE_DIR})
target_link_libraries(Test sfml-graphics sfml-audio sfml-window sfml-system GL)
