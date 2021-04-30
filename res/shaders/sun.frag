#version 330 core

in vec2 v_texCoord;

uniform mat4 u_mvp;
uniform sampler2D u_tex;

out vec4 FragColor;

void main()
{
    FragColor = texture(u_tex, v_texCoord);
}