#version 400


#extension GL_ARB_conservative_depth : enable 

in vec3 			pos_color;
in vec3				camPos;
in vec2 			uv;
in vec3 			normal;

uniform vec3 		diffuse;
uniform vec3 		specular;
uniform vec3 		ambiant;

uniform int num_light;
uniform vec3		lights[12 * 4];

uniform sampler2D	textDiffuse;

uniform int 		has_diffuse;

uniform int		id;
uniform int		meshName;

layout(location = 0) out vec4 FragColor;

vec3 compute_diffuse(vec3 pos, vec3 norm, vec3 col)
{
	int i = 0;

	vec3 ray;
	vec3 outter = vec3(0, 0, 0);

	while (i < num_light)
	{
		ray = normalize(lights[i * 4] - pos);
		outter += max(dot(ray, norm), 0.2) * lights[i * 4 + 1];
		i++;
	}
	outter.x = min(1, outter.x);
	outter.y = min(1, outter.y);
	outter.z = min(1, outter.z);
	return (outter * col);
}

vec3 compute_specular(vec3 col, vec3 viewDir, vec3 pos, vec3 norm, float specS)
{
	int i = 0;

	vec3 ray;
	vec3 reflectray;
	vec3 outter = vec3(0, 0, 0);
	float phong;

	while (i < num_light)
	{
		ray = normalize(lights[i * 4] - pos);
		reflectray = reflect(-ray, norm);
		phong = pow(max(dot(viewDir, reflectray), 0.0), specS);
		phong *= max(0, dot(ray, norm));
		outter += phong * col * lights[i * 4 + 1];
		i++;
	}
	outter.x = min(1, outter.x);
	outter.y = min(1, outter.y);
	outter.z = min(1, outter.z);
	return (outter);
}

void main(void)
{
	vec3 diffuse_color;
	vec3 frag_Normal = normalize(normal);

	if (has_diffuse == 0)
		diffuse_color = diffuse;
	else
		diffuse_color = texture(textDiffuse, uv).rgb;

	FragColor = vec4(compute_diffuse(pos_color, frag_Normal, diffuse_color).xyz +
		compute_specular(specular, normalize(camPos + pos_color), pos_color, frag_Normal, 16)
		+ 0,
		1);
}
