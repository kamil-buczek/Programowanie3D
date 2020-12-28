//
// Created by pbialas on 25.09.2020.
//
#include "app.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <glm/glm.hpp>


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

    //Animacja_piramidy
    pyramid_ = std::make_shared<Pyramid>();
    ksiezyc_ = std::make_shared<Pyramid>();
    satelita_ = std::make_shared<Pyramid>();

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glUseProgram(program);
    //Animacja Piramidy
    start_ = std::chrono::steady_clock::now();
    rotation_period  = 4.0;
    //------------------
}

void SimpleShapeApplication::frame() {

    //Ziemia----------------------------------------------------
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(now - start_).count();
    auto rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/rotation_period;
    glm::vec3 axis = glm::vec3(0.0f, 0.0f, 1.0f);
    auto R = glm::rotate(glm::mat4(1.0f), rotation_angle,axis);
    auto orbital_rotation_period = 20.0f;
    auto orbital_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/orbital_rotation_period;
    auto a = 10;
    auto b = 8;
    auto x = a*cos(orbital_rotation_angle);
    auto y = b*sin(orbital_rotation_angle);
    auto O = glm::translate(glm::mat4(1.0f), glm::vec3{x,y, 0.0});
    auto M = O*R;

    glm::mat4 P_ = camera_->projection();
    glm::mat4 V_ = camera_->view();
    auto PVM = P_*V_*M;
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    pyramid_->draw();
    //-----------------------------------------------------------------------------
    //Ksiezyc----------------------------------------------------------------------
    moon_rotation_period = 10.0;
    auto moon_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/moon_rotation_period;
    auto moon_orbital_rotation_period = 10.0;
    auto moon_orbital_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/rotation_period;
    a = 3;
    b = 3;
    x = a*cos(moon_orbital_rotation_angle);
    y = b*sin(moon_orbital_rotation_angle);
    glm::vec3 moon_axis = glm::vec3(0.0f, 0.0f, 1.0f);
    auto R_moon = glm::rotate(glm::mat4(1.0f), moon_rotation_angle,moon_axis);
    auto O_moon = glm::translate(glm::mat4(1.0f), glm::vec3{x,y, 0.0});
    PVM = P_*V_*O*O_moon*R;
    PVM = glm::scale(PVM,{0.5f,0.5f,0.5f});
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    ksiezyc_->draw();
    //--------------------------------------------------------------------------
    //Satelita------------------------------------------------------------------
    satelite_rotation_period = 2.0;
    auto satelite_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/satelite_rotation_period;
    glm::vec3 satelite_axis = glm::vec3(0.0f, 0.0f, 1.0);
    auto R_satelite = glm::rotate(glm::mat4(1.0f), satelite_rotation_angle,satelite_axis);


    auto satelite_orbital_rotation_period = 2.0;
    auto satelite_orbital_rotation_angle = 2.0f*glm::pi<float>()*elapsed_time/satelite_orbital_rotation_period;
    a = 1.5;
    b = 1.5;
    x = a*cos(satelite_orbital_rotation_angle);
    y = b*sin(satelite_orbital_rotation_angle);
    auto O_satelite = glm::translate(glm::mat4(1.0f), glm::vec3{x,y, 0.0f});


    PVM = P_*V_*O*O_satelite*R_satelite;
   //PVM = P_*V_*O*O_satelite*R_satelite;
    PVM = glm::scale(PVM,{0.25f,0.25f,0.25f});
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    satelita_->draw();
    //--------------------------------------------------------------------------

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

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}