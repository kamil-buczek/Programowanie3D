#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 a_vertex_color;

layout(std140) uniform Transformations {
   // mat4 P;
   // mat4 V;
   // mat4 M;
    mat4 PVM;

};
//mat4 PVM = P*V*M;
out vec4 vertex_color;


void main() {
    vertex_color=a_vertex_color;
    gl_Position = PVM*a_vertex_position;
    //gl_Position = a_vertex_position;

}
