cmake_minimum_required(VERSION 3.22)
include(FetchContent)
project(lvgl_sdl C)

set(CMAKE_C_STANDARD 99)


include_directories(.)
aux_source_directory(${CMAKE_SOURCE_DIR} SRC)


FetchContent_Declare(
        lvgl
        GIT_REPOSITORY https://github.com/lvgl/lvgl.git
        GIT_TAG v8.3.4
)
FetchContent_MakeAvailable(lvgl)
set(LV_CONF_PATH
        lv_conf.h
        CACHE STRING "" FORCE)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sUSE_SDL=2 --shell-file ${CMAKE_SOURCE_DIR}/shell_minimal.html")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sUSE_SDL=2 --shell-file ${CMAKE_SOURCE_DIR}/shell_minimal.html")
        set(CMAKE_EXECUTABLE_SUFFIX ".html")
else ()
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
endif ()

add_executable(${PROJECT_NAME}  ${SRC})


target_link_libraries("${PROJECT_NAME}" lvgl::lvgl)
target_link_libraries("${PROJECT_NAME}" lvgl::demos)
target_link_libraries("${PROJECT_NAME}" lvgl::examples)


if (${CMAKE_SYSTEM_NAME} STREQUAL "Emscripten")
else ()
    target_link_libraries(${PROJECT_NAME}  ${SDL2_LIBRARIES})
endif ()


get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()