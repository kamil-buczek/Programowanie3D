cmake_minimum_required(VERSION 3.15)
project(Specular)

set(CMAKE_CXX_STANDARD 14)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}" PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${PROJECT_NAME}
        main.cpp
        app.h
        app.cpp
        quad.cpp
        quad.h
        $<TARGET_OBJECTS:glad>
        camera.h rotation.h camera_controler.h phong_material.h)

target_link_libraries(${PROJECT_NAME} PUBLIC )
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

