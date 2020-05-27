#version 330

out vec4 color_out;
uniform sampler2D Image;
uniform vec4 Color;
in vec2 vTexCoords;

void main()
{
	vec4 Mytexture = texture2D(Image, vTexCoords);
	if(Mytexture.a < 0.1)
		discard;
	color_out = Mytexture * Color;
	color_out.a = Mytexture.x;
	//if(color_out.a < 0.1)
	//	discard;
}
