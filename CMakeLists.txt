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

file(GLOB_RECURSE nlohmann
    ${DEPEN_DIR}/nlohmann/*.h
    ${DEPEN_DIR}/nlohmann/*.cpp
    ${DEPEN_DIR}/nlohmann/*.hpp
)

file(GLOB_RECURSE HACD
    ${DEPEN_DIR}/HACD_Lib/inc/*.h
    ${DEPEN_DIR}/HACD_Lib/inc/*.cpp
    ${DEPEN_DIR}/HACD_Lib/src/*.cpp
)

file(GLOB sources
    include/*.h
    include/*.hpp
    src/*.cpp
    src/*.c
)

include_directories("include" "depen" "depen/HACD_Lib/inc/")

add_executable(${PROJECT_NAME} ${glm} ${nlohmann} ${HACD} ${sources} ${PROJECT_CPP_FILES} ${PROJECT_C_FILES} ${PROJECT_INC_FILES} ${PROJECT_INL_FILES})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(${PROJECT_NAME} pthread Xcursor X11 Xxf86vm Xrandr Xi GL dl)
