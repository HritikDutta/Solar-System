#version 330 core

layout (location = 0) in vec4 vertex;

uniform mat4 u_model;
uniform mat4 u_vp;
uniform sampler2D u_atlas;

out vec2 v_texCoord;

void main()
{
    gl_Position = u_vp * u_model * vec4(vertex.xy, 0, 1);
    v_texCoord  = vertex.zw;
}