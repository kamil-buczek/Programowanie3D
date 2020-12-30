//
// Created by pbialas on 25.09.2020.
//
#include "app.h"
#include <iostream>
#include <vector>
#include <tuple>


void SimpleShapeApplication::init() {

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

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
    //Zoomowanie-------------------------------------------------------------------------------------------------------
    set_camera(new Camera);

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

    int w, h;
    std::tie(w, h) = frame_buffer_size();

    camera_->perspective(glm::pi<float>()/2.0,(float)w/(float)h,0.1f,200.0f);
    glm::mat4 P_ = camera_->projection();
    camera_->look_at(glm::vec3{0.0,0.0,10.0},glm::vec3{0.0f,0.0f,0.0f},glm::vec3{1.0,0.0,0.0});
    glm::mat4 V_ = camera_->view();
    glm::mat4 M(1.0f);

    //Ruch kamery-----------------
    set_controler(new CameraControler(camera()));
    //---------------------------

    quad_ = new Quad();
    //quad_ = std::make_shared<Quad>();
    //pyramid_ = std::make_shared<Pyramid>();


    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glUseProgram(program);

    //Teksturowanie-----------------------
    auto  u_diffuse_map_location = glGetUniformLocation(program,"diffuse_map");
    if(u_diffuse_map_location==-1) {
        std::cerr<<"Cannot find uniform diffuse_map\n";
    } else {
        glUniform1ui(u_diffuse_map_location,0);
    }
    //-----------------------------------

}

void SimpleShapeApplication::frame() {


    glm::mat4 P_ = camera_->projection();
    glm::mat4 V_ = camera_->view();
    auto PVM = P_*V_;
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //pyramid_->draw();
    quad_->draw();
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    //aspect_ = (float) w / h;
    camera_->set_aspect((float) w/h);
    glm::mat4 P_ = camera_->projection();
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }

}

void SimpleShapeApplication::cleanup() {
    delete quad_;
}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}
