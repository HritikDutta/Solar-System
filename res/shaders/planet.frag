#version 460 core

in vec3 v_normal;
in vec3 v_fragPos;
in vec2 v_texCoord;

layout (std140, binding = 0) uniform Camera
{
    mat4 u_vp;
};

uniform vec3 u_lightPos;
uniform mat4 u_model;
uniform sampler2D u_tex;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(v_normal);

    vec3 lightDir = normalize(u_lightPos - v_fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    FragColor = vec4(diffuse, 1.0) * texture(u_tex, v_texCoord);
}