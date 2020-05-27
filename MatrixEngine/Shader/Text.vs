#version 330

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

uniform mat4 mvp;
out vec2 vTexCoords;

void main()
{
	vTexCoords = uv;
	gl_Position = mvp * vec4(position, 1.0f, 1.0f);
}