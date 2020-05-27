#version 330

out vec4 color_out;

in vec2 vTexCoords;

uniform sampler2D atlas;
uniform vec2 StartPos;
uniform vec2 EndPos;
uniform vec4 Color;

void main()
{
	float x_ = EndPos.x - StartPos.x;
	float y_ = EndPos.y - StartPos.y;
	
	float new_u = StartPos.x + vTexCoords.x * x_;
	float new_v = StartPos.y + vTexCoords.y * y_;
	
	vec2 new_uv = vec2(new_u, new_v);
	vec4 texture = texture2D(atlas, new_uv);
	
	color_out = texture * Color;
}
