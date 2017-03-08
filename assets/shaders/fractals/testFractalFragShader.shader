#version 400

#define PI 3.14159265

uniform vec2 resolution;
uniform mat4 view;
uniform float time;

out vec4 color;

const float MinimumDistance = 0.01;
const int MaximumRaySteps = 40;
const int Iterations = 10;
float Power = mod(time / 50000, 7.0f) + 1.0f;


const vec3 xDir = vec3(MinimumDistance, 0, 0);
const vec3 yDir = vec3(0, MinimumDistance, 0);
const vec3 zDir = vec3(0, 0, MinimumDistance);

float s;

float DE(vec3 pos) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	int i = 0;
	for (i; i < Iterations ; i++) {
		r = length(z);
		if (r>1.999f) break ;
		
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


	s = float(i) / Iterations;/* 
	+ log(log(r))/log(1) 
	- log(log(dot(z,z)))/log(1);
*/

	return 0.5*log(r)*r/dr;
}


float trace(vec3 from, vec3 direction, out float oc) {
	float totalDistance = 0.0;
	int steps;
	for (steps=0; steps < MaximumRaySteps; steps++) {
		vec3 p = from + totalDistance * direction;
		float distance = DE(p);
		totalDistance += distance;
		if (distance < MinimumDistance) break;
	}
	oc =  1.0f-(float(steps) / float(MaximumRaySteps));
	return (totalDistance);
	//return 1.0f-(float(steps) / float(MaximumRaySteps));
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
	vec3 normal;
	float oc;

	from.x  = -from.x;
	vec3 direction;
	vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
	p.x *= resolution.x/resolution.y;
	direction = normalize(vec3(p.x,p.y, 1));
		direction = get_dir(direction);
	//direction = normalize((vec4(direction.xyz, 0) * view).xyz);
	float a = trace(from, direction, oc);

	normal = get_normal(from + direction * a);
	color.xyz = vec3(Power / 10, Power * Power / 200, 0.8) * max(dot(normal, vec3(0, 1, 0)), 0) / oc;// * (abs(s) * 5);
	//color = vec4(normal.x, normal.y, normal.z, a);
	color.w = mod(oc, 1);
	if (a >  100)
		discard ;
//	color = vec4(1, 1, 1, 0);
}
