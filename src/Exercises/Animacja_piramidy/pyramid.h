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


class Pyramid  {
public:
    Pyramid();
    ~Pyramid();
    void draw();

private:
    GLuint vao_;
    GLuint buffer_[2];
};