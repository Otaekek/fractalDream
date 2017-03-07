#version 400

#extension GL_ARB_conservative_depth : enable

in vec2 uv;

out vec4 color;

uniform sampler2D renderedTexture;

void main(void)
{
	color = vec4(texture(renderedTexture, uv).rgba);
}
