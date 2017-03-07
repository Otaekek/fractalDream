#version 400

layout(location = 0) in vec3 in_Position;

out vec2 uv;

void main(void)
{
	gl_Position = vec4(in_Position.xyz, 1);
	uv = (in_Position.xy+vec2(1,1))/2.0;
}
