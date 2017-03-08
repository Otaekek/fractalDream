#version 400

#define PI 3.14159265

uniform vec2 resolution;
uniform mat4 view;
uniform float time;

const vec3 xDir = vec3(1, 0, 0);
const vec3 yDir = vec3(0, 1, 0);
const vec3 zDir = vec3(0, 0, 1);

out vec4 color;

const float MinimumDistance = 0.001;
const int MaximumRaySteps = 100;
const int Iterations = 50;
float tmp = mod(time / 50000, 6.0f) + 2;
float Power = tmp;
float k = mod(time / 100000, 1.0f) / 2;

float DE(vec3 pos) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < Iterations ; i++) {
		r = length(z);
		if (r>1.999f) break;
		
		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, Power-1.0)*Power*dr + 1.0;
		
		// scale and rotate the point
		float zr = pow( r,Power);
		theta = theta*Power;
		phi = phi*Power;
		
		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=pos;
	}
	return 0.5*log(r)*r/dr;
}

float trace(vec3 from, vec3 direction) {
	float totalDistance = 0.0;
	int steps;
	for (steps=0; steps < MaximumRaySteps; steps++) {
		vec3 p = from + totalDistance * direction;
		float distance = DE(p);
		totalDistance += distance;
		if (distance < MinimumDistance) break;
	}
	return 1.0f-(float(steps) / float(MaximumRaySteps));
}

vec3 get_normal(vec3 pos)
{
	return (normalize(vec3(DE(pos+xDir)-DE(pos-xDir),
		DE(pos+yDir)-DE(pos-yDir),
		DE(pos+zDir)-DE(pos-zDir))));
}

vec3 get_dir(vec3 dir)
{
	return (vec4(dir.xyz, 0) * view).xyz;
}

vec3 get_pos()
{
	vec4 tmpcamPos = vec4(0, 0, 0, 1);
	return (tmpcamPos * view).xyz;
//	return (tmpcamPos *view).xyz;
	tmpcamPos.x = -view[0][3];
	tmpcamPos.y = -view[1][3];
	tmpcamPos.z = -view[2][3];
	tmpcamPos.w = 1;
//	tmpcamPos *= inverse(view);
	return tmpcamPos.xyz;	
}

void main(void)
{
	vec3 from = get_pos();
	from.x  = -from.x;
	vec3 direction;
	vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
	if (tmp > 5.0f)
		Power = 10.0f - tmp;

	p.x *= resolution.x/resolution.y;
	direction = normalize(vec3(p.x,p.y, 1));
		direction = get_dir(direction);
	//direction = normalize((vec4(direction.xyz, 0) * view).xyz);
	float a = trace(from, direction);
	color = vec4(vec3((1 - k + 0.5), k + k, k) * a * a * a * a * a* a, a);
	if (a == 0)
		discard ;
//	color = vec4(1, 1, 1, 0);
}
