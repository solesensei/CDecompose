cmake_minimum_required(VERSION 3.5)
project(CDecompose)

set(CMAKE_CXX_STANDARD 11)
set(CMALE_CXX_FLAGS "-O3")
set(DEPEN_DIR "depen")

file(GLOB_RECURSE glm
    ${DEPEN_DIR}/glm/*.h
    ${DEPEN_DIR}/glm/*.cpp
    ${DEPEN_DIR}/glm/*.hpp
)

file(GLOB_RECURSE glad
    ${DEPEN_DIR}/GLAD/*.h
)

file(GLOB glfw
    ${DEPEN_DIR}/GLFW/*.h
)

file(GLOB SOIL
    ${DEPEN_DIR}/SOIL/*.h
)

file(GLOB sources
    include/*.h
    include/*.hpp
    src/*.cpp
    src/*.c
)

include_directories("include" "depen" "depen/GLAD")

add_executable(${PROJECT_NAME} ${glm} ${glad} ${glfw} ${sources} ${SOIL})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)


link_directories("static")
target_link_libraries(${PROJECT_NAME} glfw pthread Xinerama Xcursor X11 Xxf86vm Xrandr Xi GL dl SOIL)
