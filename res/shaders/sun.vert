#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 texCoord;

uniform mat4 u_mvp;
uniform sampler2D u_tex;

out vec2 v_texCoord;

void main()
{
    gl_Position = u_mvp * vec4(aPos, 1.0);
    v_texCoord = texCoord;
}