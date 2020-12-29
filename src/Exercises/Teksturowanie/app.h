//
// Created by pbialas on 05.08.2020.
//
#pragma once
#include <vector>
#include <glm/mat4x4.hpp> // glm::mat4
#include <memory>
#include <chrono>
#include "Application/application.h"
#include "Application/utils.h"
#include "glad/glad.h"
#include "camera.h"
#include "camera_controler.h"
#include "pyramid.h"

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;
    void frame() override;
    void mouse_button_callback(int button, int action, int mods) override;
    void cursor_position_callback(double x, double y) override;
    void framebuffer_resize_callback(int w, int h) override;
    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera_;
    std::shared_ptr<Pyramid> pyramid_;

    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(yoffset / 30.0f);
    }

    void set_controler(CameraControler *controler) { controler_ = controler; }
    Camera *camera() { return camera_; }

private:
    GLuint vao_;
    GLuint u_pvm_buffer_;
    CameraControler *controler_;

};