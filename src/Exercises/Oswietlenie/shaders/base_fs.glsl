#version 410

in vec2 v_vertex_texture_coordinates;

layout(std140) uniform Modifiers {
    float light_intensity;
    vec3 light_color;
};

layout(location=0) out vec4 vFragColor;
uniform sampler2D color;
uniform sampler2D diffuse_map;

void main() {
    vFragColor = texture(diffuse_map, v_vertex_texture_coordinates);
}
