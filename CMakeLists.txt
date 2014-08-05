cmake_minimum_required(VERSION 2.8)
project(jbms-utility)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


set (${PROJECT_NAME}_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")


#### Build tests
include_directories(${${PROJECT_NAME}_INCLUDE_DIRS})

#### Install headers
set(header_install_dir "include/${PROJECT_NAME}")
install(DIRECTORY "${${PROJECT_NAME}_INCLUDE_DIRS}" DESTINATION "${header_install_dir}")

#### Generate CMake modules for importing this package
include(jbms-WriteConfig)
write_config_file(
  TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in"
  PATH_VARS ${PROJECT_NAME}_INCLUDE_DIRS
  REL_PATH "${header_install_dir}" )
