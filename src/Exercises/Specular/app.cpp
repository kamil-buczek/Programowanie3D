//
// Created by pbialas on 25.09.2020.
//
#include "app.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <glm/gtx/string_cast.hpp>
#include "Application/utils.cpp"




void SimpleShapeApplication::init() {

    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");

    //PVM--------------------------------------------------------------------------------------------------------------
    xe::utils::set_uniform_block_binding(program, "Transformations", 1);

    //Oswietlenie------------------
    xe::utils::set_uniform_block_binding(program, "Light", 2);

    xe::utils::set_uniform_block_binding(program, "Material", 3);

    //-----------------------------------------------------------------------------------------------------------------
    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    set_camera(new Camera);

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
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,2 * sizeof(glm::mat4) + 12 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);

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

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    //Oświetlenie-------------------------
    glGenBuffers(1, &u_light_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer);
    glBufferData(GL_UNIFORM_BUFFER,16 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer);

    light_.ambient = glm::vec4(0.2, 0.2, 0.2, 1.0);
    light_.position = glm::vec4(0.0, 0.0, 1.0, 1.0 );
    light_.color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    light_.a = glm::vec4(1.0, 0.0, 1.0, 0.0);

    //------------------------------------
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glUseProgram(program);

    //Teksturowanie-----------------------
   // auto  u_diffuse_map_location = glGetUniformLocation(program,"diffuse_map");
   // if(u_diffuse_map_location==-1) {
   //     std::cerr<<"Cannot find uniform diffuse_map\n";
   // } else {
   //     glUniform1ui(u_diffuse_map_location,0);
   // }
    //-----------------------------------

    xe::utils::set_uniform1i(program,"diffuse_map",0);


    auto material = new PhongMaterial();
    material->Kd = glm::vec3(1.0, 0.0, 0.0);
    material->Kd_map = 0;
    material->Ks = glm::vec3(0.05, 0.05, 0.05);
    material->Ks_map = 0;
    material->Ns = 1000.0f;
    material->Ns_map = 0;
    quad_->set_material(material);

    glGenBuffers(1, &u_material_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER,u_material_buffer);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(PhongMaterial), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 3, u_material_buffer);


    GLuint diffuse_texture;
    glGenTextures(1,&diffuse_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture);
    xe::utils::load_texture(std::string(PROJECT_DIR) + "/Textures/plastic.png");
    glBindTexture(GL_TEXTURE_2D, 0);

    material->Kd = glm::vec3(1.0, 1.0, 1.0);
    material->Kd_map = diffuse_texture;

    xe::utils::set_uniform1i(program,"specular_map",1);
    xe::utils::set_uniform1i(program,"shininess_map",2);

    GLuint shininess_texture;
    glGenTextures(1,&shininess_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shininess_texture);
    xe::utils::load_texture(std::string(PROJECT_DIR) + "/Textures/shininess.png");
    glBindTexture(GL_TEXTURE_2D, 0);

    material->Ns = 1000.0f;
    material->Ns_map = shininess_texture;

}

void SimpleShapeApplication::frame() {

    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    auto P = camera()->projection();
    auto VM = camera()->view();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]);

    auto R = glm::mat3(VM);
    auto N = glm::transpose(glm::inverse(R));

    for (int col = 0; col < 3; col++){
        glBufferSubData(GL_UNIFORM_BUFFER,
                        2 * sizeof(glm::mat4) + col * 4 * sizeof(GLfloat),
                        3 * sizeof(GLfloat), &N[col]);
    }
    auto light_position_in_vs = camera()->view() * light_.position;
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    light_.position_in_vs = camera()->view() * light_.position;
    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), &light_.position_in_vs[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, u_material_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(PhongMaterial), quad_->material());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //pyramid_->draw();



    auto material = quad_->material();
    if(material->Kd_map>0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->Kd_map);
    }

    if(material->Ks_map>0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, material->Ks_map);
    }
    if(material->Ns_map>0) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, material->Ns_map);
    }

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
