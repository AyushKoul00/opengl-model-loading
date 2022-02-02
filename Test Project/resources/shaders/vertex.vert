#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 7) in vec3 aColor;


out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
#ifdef VERTEX_COLOR
out vec3 Color;
#endif

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 WorldPos = model * vec4(aPos, 1.0);
    FragPos = WorldPos.xyz;
    Normal = aNormal;
    TexCoords = aTexCoords;
    #ifdef VERTEX_COLOR
    Color = aColor;
    #endif
    gl_Position = projection * view * WorldPos;
}