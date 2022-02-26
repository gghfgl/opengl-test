#shader vertex
#version 450 core

layout (location = 0) in vec2 position; // <vec2 position>

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
}

#shader fragment
#version 450 core

out vec4 color;

void main()
{
    color = vec4(0.0, 1.0, 0.0, 1.0);
}