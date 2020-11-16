#pragma once
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:

    void look_at(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
        V_ = glm::lookAt(eye, center, up);
    }

    void perspective(float fov, float aspect, float near, float far) {
        fov_ = fov;
        aspect_ = aspect;
        near_ = near;
        far_ = far;
    }

    void set_aspect(float aspect) {
        aspect_ = aspect;
    }

    glm::mat4 view() const { return V_; }

    glm::mat4 projection() const { return glm::perspective(fov_, aspect_, near_, far_); }


private:
    float fov_;
    float near_;
    float far_;
    float aspect_;
    glm::mat4 V_;
};