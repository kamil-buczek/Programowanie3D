#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec2 a_vertex_texture_coordinates;
layout(location=2) in  vec3 a_vertex_normal;
out vec2 v_vertex_texture_coordinates;

layout(std140) uniform Transformations {
    mat4 PVM;
};

void main() {
    v_vertex_texture_coordinates=a_vertex_texture_coordinates;
    gl_Position = PVM*a_vertex_position;
}
