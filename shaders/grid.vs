#version 330 core

layout (location = 0) in vec3 cPos;

void main()
{
    gl_Position = vec4(cPos.x, cPos.y, cPos.z, 1.0f);
}
