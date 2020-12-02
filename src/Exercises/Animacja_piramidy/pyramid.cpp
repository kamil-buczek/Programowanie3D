//
// Created by Kamil Buczek on 02.12.2020.
//

#include "pyramid.h"
#include <vector>
#include <iostream>
#include <tuple>



Pyramid::Pyramid() {
    /*
        Tu dodajemy kod tworzący bufory indeksów i wierzchołków oraz obiekt VAO,
        Potem ładujemy dane wierzchołków i indeksow do buforów  i  podłaczamy je do atrybutów w szaderze
   */

    //Tablica wierzchołków i kolorów
    std::vector<GLfloat> domek = {

            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.8f, 1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.8f, 0.0f, 1.0f, 0.0f,

            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.8f, 0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 0.0f, 0.8f, 0.0f, 1.0f, 1.0f,
    };

    //Bufor indeksów
    std::vector<GLushort> indices = {
            2,0,1,1,3,2,6,5,4,9,8,7,12,11,10,15,14,13 // wypisujemy tyle elementów ile mamy wierzchołków
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
    glBufferData(GL_ARRAY_BUFFER, domek.size() * sizeof(GLfloat), domek.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_[1]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[0]); //dodane w zadaniu 3
    glBindVertexArray(0);
}

Pyramid::~Pyramid() {
    //Tu usuwamy VAO i bufory
    glDeleteBuffers(1,buffer_);
    glDeleteVertexArrays(1,&vao_);
}

void Pyramid::draw() {
    //Tu wywolujemy polecenie glDrawElements
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(0));
    glBindVertexArray(0);
}