cmake_minimum_required(VERSION 3.5)

project(SimpleMultiCopy LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(SimpleMultiCopy
  main.cpp
  qmainwidget.cpp
  thewidgetitem.cpp
  copythread.cpp
  dragdroplistwidget.cpp
  qmainwidget.h
  thewidgetitem.h
  copythread.h
  dragdroplistwidget.h
  qmainwidget.ui
  thewidgetitem.ui
)

target_link_libraries(SimpleMultiCopy PRIVATE Qt5::Widgets)
