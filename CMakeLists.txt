cmake_minimum_required(VERSION 3.14)

project(Jan-Itor
  VERSION 1.0.0
  DESCRIPTION "Terminal app to clean and serialize single word responses"
  LANGUAGES CXX
)

option(SYSTEM_FTXUI "Use the system's ftxui libraries" OFF)

if(SYSTEM_FTXUI)
   find_package(ftxui)
else()
   #Fetch content
   include(FetchContent)
   set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
   set(FETCHCONTENT_QUIET FALSE)
   FetchContent_Declare(ftxui
     GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
     GIT_TAG v5.0.0
     GIT_PROGRESS TRUE
   )
   FetchContent_GetProperties(ftxui)
   if(NOT ftxui_POPULATED)
     FetchContent_Populate(ftxui)
     add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
   endif()
endif()

#Project

# ---- Declare library ----

add_library(Jan-Itor_lib 
  src/Serial.cpp
  src/Serial.hpp
  src/IpForm.cpp
  src/IpForm.hpp
  src/MyCol.hpp
  src/MyCSV.cpp
  src/MyCSV.hpp
)
add_library(Menu_lib
  src/Components.hpp
  src/TablePreview.cpp
  src/DelimDropDown.cpp
  src/ListSelect.cpp
  src/InputContainer.cpp
  src/IpFormMenu.hpp
  src/MyCSVMenu.hpp
  src/SerialMenu.hpp
)
add_executable(Jan-Itor 
  src/main.cpp
)

#target_include_directories(Jan-Itor PRIVATE src)
target_link_libraries(Menu_lib
  PRIVATE Jan-Itor_lib
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
target_link_libraries(Jan-Itor
  PRIVATE ftxui::screen
  PRIVATE Jan-Itor_lib
  PRIVATE Menu_lib
)
target_compile_features(Jan-Itor_lib PUBLIC cxx_std_17)
target_compile_features(Jan-Itor PUBLIC cxx_std_17)
target_include_directories(Jan-Itor_lib SYSTEM INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(Jan-Itor     SYSTEM INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(Jan-Itor_lib
   PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
   PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
)
target_include_directories(Jan-Itor
   PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
   PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
)
