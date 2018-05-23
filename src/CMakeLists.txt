# OpenCV依赖
find_package(OpenCV REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# GLUT
find_package(GLUT REQUIRED)

# Glew
find_package(Glew REQUIRED)

#CUDA
find_package(CUDA REQUIRED)

if (NOT CUDA_FOUND)
  message(STATUS "CUDA is not found. Project will not be built.")
endif(NOT CUDA_FOUND)

if (WIN32)
  set(EXTRA_CXX_FLAGS "/DVERBOSE /D_CRT_SECURE_NO_WARNINGS ")
  list(APPEND CUDA_NVCC_FLAGS "-arch=sm_50;--compiler-options;-O2;-DVERBOSE") 
endif()
if (UNIX)
  if (APPLE)
    set(EXTRA_CXX_FLAGS "-DVERBOSE -msse2")
    list(APPEND CUDA_NVCC_FLAGS "-arch=sm_50;--compiler-options;-O2;-DVERBOSE") 
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -msse2 ")
    list(APPEND CUDA_NVCC_FLAGS "-lineinfo;--compiler-options;-O2;-DVERBOSE") 
  endif()
endif()

SET(CUDA_SEPARABLE_COMPILATION ON)

# set SiftGPU header and library location
include_directories( ${PROJECT_BINARY_DIR}/include/SiftGPU ${OpenGL_INCLUDE_DIR} )
set( SIFTGPU_LIBS ${PROJECT_BINARY_DIR}/bin/libsiftgpu.so )

add_library( Fusion Preprocess.cpp CUtil.cpp CFusion.cpp CStitching.cpp CFuImage.cpp )
add_executable( material_fusion main.cpp)
add_executable( testpro mytest.cpp)

target_link_libraries( material_fusion
    Fusion
    ${OpenCV_LIBS}
    ${SIFTGPU_LIBS}
    ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES}
)

target_link_libraries( testpro
    Fusion
    ${OpenCV_LIBS}
    ${SIFTGPU_LIBS}
    ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES}
)


set_target_properties(material_fusion
  PROPERTIES
  COMPILE_FLAGS "${EXTRA_CXX_FLAGS}"			   
)

set_target_properties(testpro
  PROPERTIES
  COMPILE_FLAGS "${EXTRA_CXX_FLAGS}"			   
)