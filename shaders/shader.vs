#version 330 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aCol;

//out vec3 col;

uniform mat4 u_MVP;

void main()
{
    //col = aCol;
    gl_Position = u_MVP * vec4(aPos, 1.0);
}
