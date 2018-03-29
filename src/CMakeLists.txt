# OpenCV依赖
find_package( OpenCV REQUIRED )

# OpenGL
find_package(OpenGL REQUIRED)

# GLUT
find_package(GLUT REQUIRED)

# Glew
find_package(Glew REQUIRED)

# SiftGPU：手动设置其头文件与库文件所在位置
include_directories("/media/cwh1001/Workspace/SiftGPU/src/SiftGPU" ${OpenGL_INCLUDE_DIR})
set(SIFTGPU_LIBS "/media/cwh1001/Workspace/SiftGPU/bin/libsiftgpu.so")

add_executable( material_fusion main.cpp preprocess.cpp)

target_link_libraries( material_fusion

    ${OpenCV_LIBS}
    ${SIFTGPU_LIBS}
    ${GLEW_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES}
)