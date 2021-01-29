#version 410

in vec2 v_vertex_texture_coordinates;
in vec3 vertex_normal_in_vs;
in vec3 vertex_position_in_vs;

layout(std140) uniform Modifiers {
    float light_intensity;
    vec3 light_color;
};

layout(std140) uniform Light {
    vec3 ambient;
    vec3 position_in_vs;
    vec3 color;
    vec3 a;
} light;

layout(location=0) out vec4 vFragColor;
uniform sampler2D color;
uniform sampler2D diffuse_map;

vec3 specular_color = vec3(1.0, 1.0, 1.0);
float shininess = 500.0;

void main() {



    vec3 normal = normalize(vertex_normal_in_vs);
    vec3 light_vector = light.position_in_vs - vertex_position_in_vs;
    float r = length(light_vector);
    light_vector/=r;

    vec4 diffuse_color = texture(diffuse_map, v_vertex_texture_coordinates);

    float M_PI = 3.1415926;
    diffuse_color.rgb/=M_PI;


    //if (!gl_FrontFacing) {
    //    normal = -normal;
    //}


    float attenuation = 1.0/(light.a[0]+light.a[1]*r+light.a[2]*r*r);
    float light_in = max(0.0, dot(normal, light_vector))*attenuation;

    float specular = 0.0;
    if (r>0){
        vec3 view_vector = -normalize(vertex_position_in_vs);
        vec3 half_vector = normalize(light_vector + view_vector);
        specular = ((shininess + 8.0)/(8.0*M_PI))*pow(max(0.0, dot(half_vector, normal)), shininess);
    }



    vFragColor.a = diffuse_color.a;
    vFragColor.rgb = diffuse_color.rgb;
    //vFragColor.rgb = diffuse_color.rgb*light.ambient.rgb;
    vFragColor.rgb += light_in * diffuse_color.rgb * light.color;
    vFragColor.rgb += light_in*light.color*specular;

}
