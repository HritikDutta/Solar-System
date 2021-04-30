#version 330 core

in vec3 v_normal;
in vec3 v_fragPos;
in vec2 v_texCoord;

uniform vec3 u_sunToEarthVector;
uniform mat4 u_model;
uniform mat4 u_vp;
uniform sampler2D u_dayTex;
uniform sampler2D u_nightTex;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(v_normal);
    float factor = dot(norm, normalize(u_sunToEarthVector));

    FragColor = texture(u_dayTex, v_texCoord) * factor +
                texture(u_nightTex, v_texCoord) * (1 - factor);
}