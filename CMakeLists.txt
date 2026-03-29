cmake_minimum_required(VERSION 3.16)
project(AsciiConverter VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Find OpenCV
find_package(OpenCV REQUIRED)

add_executable(AsciiConverter
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(AsciiConverter PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${OpenCV_LIBS}
)