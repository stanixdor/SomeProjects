#version 330

out vec4 color_out;
uniform sampler2D Image;
uniform vec4 Color;
in vec2 vTexCoords;

void main()
{
	vec4 Mytexture = texture2D(Image, vTexCoords);
	color_out = Mytexture * Color;
}
