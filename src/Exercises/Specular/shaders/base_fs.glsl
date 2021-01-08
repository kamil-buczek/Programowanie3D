#version 410

in vec2 v_vertex_texture_coordinates;
in vec3 vertex_normal_in_vs;
in vec3 vertex_position_in_vs;

layout(std140) uniform Modifiers {
    float light_intensity;
    vec3 light_color;
};

layout(std140) uniform Light {
    vec3 position_in_vs;
    vec3 color;
    vec3 a;
    vec3 ambient;
} light;



layout(location=0) out vec4 vFragColor;
uniform sampler2D color;
uniform sampler2D diffuse_map;

vec3 specular_color = vec3(1.0, 1.0, 1.0); //Specular
float shininess = 500.0; //Specular

void main() {

    vec3 view_vector = (-1.0 * vertex_position_in_vs);  //Specular
    view_vector = normalize(view_vector); //Specular

    float M_PI = 3.1415926; //Specular
    vec4 diffuse_color = texture(diffuse_map, v_vertex_texture_coordinates);
    diffuse_color.rgb/=M_PI; //Specular

    vec3 normal = normalize(vertex_normal_in_vs);

    //if (!gl_FrontFacing) {
    //    normal = -normal;
    //}

    //vec3 light_vector = normalize(light.position_in_vs - vertex_position_in_vs);
    vec3 light_vector = light.position_in_vs - vertex_position_in_vs;
    float r = length(light_vector);
    light_vector/=r;
    float attenuation = 1.0/(light.a[0]+light.a[1]*r+light.a[2]*r*r);
    float light_in = max(0.0, dot(normal, light_vector))*attenuation;

    vec3 half_vector = normalize(light_vector + view_vector);
    float HdotN = max(0.0, dot(normal, half_vector)); //Specular OK
    float specular = ((shininess + 8)/(8*M_PI)) * (pow(HdotN,shininess)); //Specular


    vFragColor.a = diffuse_color.a;
    //vFragColor.rgb = diffuse_color.rgb*light.ambient.rgb; UWAGA TUTAJ MOŻE BYC ŹLE
    vFragColor.rgb = diffuse_color.rgb;
    //vFragColor.rgb += light_in * diffuse_color.rgb * light.color;
    vFragColor.rgb += light_in * light.color * specular; //Specular

}
