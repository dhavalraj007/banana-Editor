#version 410 core
out vec4 outColor;
in vec3 vpos;

void main()
{
    outColor = vec4(vpos,1.0);
}