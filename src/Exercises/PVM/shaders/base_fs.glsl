#version 410

in vec4 vertex_color;

layout(std140) uniform Modifiers {
    float light_intensity;
    vec3 light_color;
};

layout(location=0) out vec4 vFragColor;

void main() {
    vFragColor.a = vertex_color.a;
    vFragColor.rgb = light_intensity*light_color*vertex_color.rgb;
}
