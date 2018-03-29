cmake_minimum_required(VERSION 2.8.3)
project(material_fusion)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wno-write-strings -Wno-unused-result -Wno-deprecated -fPIC -std=c++0x")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wno-write-strings -Wno-unused-result -Wno-deprecated -fPIC")

# include directories

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}/include

  /usr/local/include
  /usr/include
)

# lib directories
LINK_DIRECTORIES(
  ${PROJECT_BINARY_DIR}/lib
  ${PROJECT_SOURCE_DIR}
  
  /usr/local/lib
  /use/lib
)

ADD_SUBDIRECTORY(src)
