#version 410 core
layout (location = 0) in vec3 position;
out vec3 vpos;
uniform vec2 offset = vec2(0.5);
uniform mat4 model = mat4(1.0);
        
void main()
{
    vpos = position + vec3(offset,0.0f);
    gl_Position = model * vec4(position,1.0f);
}