//
// Created by Kamil Buczek on 02.12.2020.
//
#pragma once
#include <vector>
#include <glm/mat4x4.hpp> // glm::mat4
#include "Application/application.h"
#include "Application/utils.h"
#include "glad/glad.h"
#include "camera.h"
#include "phong_material.h"


class Quad  {
public:
    Quad();
    ~Quad();
    void draw();
    Quad(const Quad& rhs) = delete;
    Quad &operator=(const Quad& rhs) = delete;

    void operator=(Quad&& rhs) = delete;
    Quad(Quad&& rhs) = delete;

    PhongMaterial *material_;
    void set_material(PhongMaterial *material) { material_ = material; } //setter
    PhongMaterial *material() { return material_; } //getter


private:
    GLuint vao_;
    GLuint buffer_[2];
    //Teksturowanie--------
    GLuint diffuse_texture_;
    //---------------------
};