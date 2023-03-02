#version 330 core

//in vec3 col;

out vec4 FragColor;

uniform vec3 u_faceColor;

void main()
{
    //FragColor = vec4(col, 1.0f);
    FragColor = vec4(u_faceColor, 1.0f);
}
