# OpenCV依赖
find_package( OpenCV REQUIRED )

# OpenGL
find_package(OpenGL REQUIRED)

# GLUT
find_package(GLUT REQUIRED)

# Glew
find_package(Glew REQUIRED)

# 设置SiftGPU头文件与库文件所在位置
include_directories( ${PROJECT_BINARY_DIR}/src/SiftGPU ${OpenGL_INCLUDE_DIR})
set( SIFTGPU_LIBS ${PROJECT_BINARY_DIR}/bin/libsiftgpu.so )

add_library(Fusion Preprocess.cpp CFusion.cpp)
add_executable( material_fusion main.cpp)
add_executable( testpro mytest.cpp CStitching.cpp)

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