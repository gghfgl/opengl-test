#shader vertex
#version 450 core

layout (location = 0) in vec2 vertex; // <vec2 position>

uniform mat4 projection;
uniform mat4 model;

void main()
{
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}

#shader fragment
#version 450 core

out vec4 FragColor;

uniform vec4 color;

void main()
{
    FragColor = color;
}