//
// Created by Kamil Buczek on 02.12.2020.
//

#include "quad.h"
#include <vector>
#include <iostream>
#include <tuple>
//Teksturowanie-------------------------
#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/src/stb/stb_image.h"
//--------------------------------------


Quad::Quad() {

    //Oświetlenie -- reprezentacja kwadratu
    std::vector<GLfloat> kwadrat = {
                                                    //Wektory normalne  dodanie w zadaniu z oświetleniem
            -1.0f, 1.0f, 0.0f, 0.8090f, 0.5000f,    0.0f, 0.0f, 1.0f, //0
            1.0f, 1.0f, 0.0f, 0.5000f, 0.8090f,     0.0f, 0.0f, 1.0f, //1
            -1.0f, -1.0f, 0.0f, 0.5000f, 0.1910f,   0.0f, 0.0f, 1.0f, //2
            1.0f, -1.0f, 0.0f, 0.1910f, 0.5000f,    0.0f, 0.0f, 1.0f, //3
    };

    //Bufor indeksów
    std::vector<GLushort> indices = {
            //2,0,1,1,3,2,  6,5,4,  9,8,7,  12,11,10,  15,14,13 // wypisujemy tyle elementów ile mamy wierzchołków
            2,0,1,1,3,2,
    };

    //GLuint idx_buffer_handle;
    glGenBuffers(1,&buffer_[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //GLuint v_buffer_handle;
    glGenBuffers(1, &buffer_[1]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[1]);
    glBufferData(GL_ARRAY_BUFFER, kwadrat.size() * sizeof(GLfloat), kwadrat.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[1]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));

    //Oświetlenie-----------------------------------
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(5*sizeof(GLfloat)));
    //----------------------------------------------

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[0]); //dodane w zadaniu 3
    glBindVertexArray(0);

}

Quad::~Quad() {
    //Tu usuwamy VAO i bufory
    glDeleteBuffers(1,buffer_);
    glDeleteVertexArrays(1,&vao_);
}

void Quad::draw() {
    //-----------------------------------------
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
    //Teksturowanie---------------
    glBindTexture(GL_TEXTURE_2D,0);
}