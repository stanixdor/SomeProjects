#version 330

layout(location = 0) in vec2 position;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp  * vec4(position, 0.0f, 1.0f);
}