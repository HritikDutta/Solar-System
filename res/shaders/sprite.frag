#version 330 core

in vec2 v_texCoord;

out vec4 color;

uniform mat4 u_model;
uniform mat4 u_vp;
uniform sampler2D u_atlas;

void main()
{
    color = texture(u_atlas, v_texCoord);
}