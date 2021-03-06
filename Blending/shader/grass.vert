#version 420 core

layout(location = 0) in vec3 vPosition;
layout(location = 2) in vec2 vTexCoord;

out vec2 TexCoord;

uniform mat4 model;
layout(std140, binding = 0) uniform Matrix
{
    mat4 view;
    mat4 projection;
};

void main()
{
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	TexCoord = vTexCoord;
}