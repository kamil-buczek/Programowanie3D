//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <string>

#include "glad/glad.h"

namespace xe {
    namespace utils {

        std::string load_file(const std::string &path);

        std::string get_gl_description(void);

        std::string shader_type(GLenum type);


    }

    GLuint create_shader_from_file(GLenum type, const std::string &path);

    GLuint link_program(GLuint program);

    GLuint create_program(const std::string &vs_path, const std::string &fs_path);

    void set_uniform1i(GLuint program, const std::string& name, int value); //Specular
    void set_uniform_block_binding(GLuint program, const std::string &name, GLuint binding); //Specular

    std::string error_msg(GLenum status);
    uint8_t *load_image(const std::string &filename, int *width, int *height, int *n_channels);
    void load_texture(const std::string &filename);
}