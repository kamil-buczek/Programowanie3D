//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>
#include <glm/mat4x4.hpp> // glm::mat4
#include "Application/application.h"
#include "Application/utils.h"
#include "glad/glad.h"
#include "camera.h"

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    Camera *camera_;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

private:
    GLuint vao_;
    GLuint u_pvm_buffer_;
};