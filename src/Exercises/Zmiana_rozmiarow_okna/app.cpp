//
// Created by pbialas on 25.09.2020.
//
#include "app.h"
#include <iostream>
#include <vector>
#include <tuple>
#include "Application/utils.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/string_cast.hpp>

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

    //Uniforms---------------------------------------------------------------------------------------------------------
    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
    if (u_modifiers_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
    }
    else {
        glUniformBlockBinding(program, u_modifiers_index, 0);
    }
    //-----------------------------------------------------------------------------------------------------------------
    //PVM--------------------------------------------------------------------------------------------------------------
    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX) {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    } else{
        glUniformBlockBinding(program, u_transformations_index, 1);
    }
    //-----------------------------------------------------------------------------------------------------------------

    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    //Tablica wierzchołków i kolorów
    std::vector<GLfloat> domek = {

            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,



    };

    //Bufor indeksów
    std::vector<GLushort> indices = {
           2,1,0,1,2,3,6,5,4,9,8,7,12,11,10,15,14,13 // wypisujemy tyle elementów ile mamy wierzchołków
            //4,5,6,7,8,9,10,11,12,13,14,15
    };
    GLuint idx_buffer_handle;
    glGenBuffers(1,&idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    //Vertex Buffer
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, domek.size() * sizeof(GLfloat), domek.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);

    //Ustawiania skad mają wyświetlać się wierzchołki
    glEnableVertexAttribArray(0);
    //Czyli współrzędne wierzchołka trójkąta pobierz z 3 wartości
    // zaczynając od 0 (reinterpret_cast<GLvoid *>(0)) i przeskakuj o 6 następnych (6 * sizeof(GLfloat))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    //Ustawianie skąd ma wyświetlać się kolor
    // Czyli kolor pobierz z 3 wartości zaczynając
    // od 3 (reinterpret_cast<GLvoid *>(3*sizeof(GLfloat))) i przeskakuj o 6 do następnych (6 * sizeof(GLfloat))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle); //dodane w zadaniu 3
    glBindVertexArray(0);

    //Uniform-------------------------------------------------------------------------------------
    GLuint ubo_handle;

    glGenBuffers(1,&ubo_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle); //zabindowanie
    float strength = 1.0;
    float light[3] = {0.7, 0.8, 1.0};
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), light);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); //odbindowanie
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle);
    //---------------------------------------------------------------------------------------------
    //PVM----------------------------------------------------------------------------------------------------------
    glGenBuffers(1,&u_pvm_buffer_);

    //---------------------------------------------------------------------------------------------
    int w, h;
    std::tie(w, h) = frame_buffer_size();

    aspect_ = (float)w/(float)h;
    fov_ = glm::pi<float>()/2.0;
    near_ = 1.0f;
    far_ = 10.0f;

    P_ = glm::perspective(fov_, aspect_, near_, far_);
    V_ = glm::lookAt(glm::vec3{-1.0,-1.0,1.0},glm::vec3{0.0f,0.0f,0.0f},glm::vec3{1.0,0.5,1.0});
    glm::mat4 M(1.0f);

    //auto PVM = P_*V_;
    //glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    //glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);
    //--------------------------------------------------------------------------------------------------------------

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {

    auto PVM = P_*V_;
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    glBindVertexArray(vao_);
    //glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    aspect_ = (float) w / h;
    P_ = glm::perspective(fov_, aspect_, near_, far_);
}
