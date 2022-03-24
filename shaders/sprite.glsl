#shader vertex
#version 450 core

layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}

#shader fragment
#version 450 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D image;
uniform vec3 color;

void main()
{    
    FragColor = vec4(color, 1.0) * texture(image, TexCoords);
}