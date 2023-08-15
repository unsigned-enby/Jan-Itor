cmake_minimum_required(VERSION 3.14)

#Fetch content
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v4.1.1
  GIT_PROGRESS TRUE
)
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

#Project
project(Jan-Itor
  VERSION 1.0.0
  DESCRIPTION "Terminal app to clean and serialize single word responses"
  LANGUAGES CXX
)
# ---- Declare library ----

add_library(Jan-Itor_lib 
  src/DelimDropDown.cpp
  src/DelimDropDown.hpp
  src/Serial.cpp
  src/Serial.hpp
  src/SerialMenu.cpp
  src/SerialMenu.hpp
  src/InputContainer.cpp
  src/InputContainer.hpp
  src/IpForm.cpp
  src/IpForm.hpp
  src/IpFormMenu.cpp
  src/IpFormMenu.hpp
  src/MyCSV.cpp
  src/MyCSV.hpp
  src/MyCSVMenu.cpp
  src/MyCSVMenu.hpp
)
add_executable(Jan-Itor 
  src/main.cpp
)

#target_include_directories(Jan-Itor PRIVATE src)
target_link_libraries(Jan-Itor_lib
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
target_link_libraries(Jan-Itor 
  PRIVATE Jan-Itor_lib
  PRIVATE ftxui::screen
)
include (cmake/options.cmake)
Jan_set_option(Jan-Itor_lib)
Jan_set_option(Jan-Itor)
