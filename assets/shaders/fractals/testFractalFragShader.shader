#version 400

float PI=3.14159265;

uniform vec2 resolution;


out vec4 color;


const float MinimumDistance = 0.00000001;
const float MaximumRaySteps = 60;
const int Iterations = 3;
const float Scale = 1;
const float Power = 7;


float DE(vec3 pos) {
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < Iterations ; i++) {
		r = length(z);
		if (r>100) break;
		
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

/*
float DE(vec3 z)
{
    float r;
    int n = 0;
    while (n < Iterations) {
       if(z.x+z.y<0) z.xy = -z.yx; // fold 1
       if(z.x+z.z<0) z.xz = -z.zx; // fold 2
       if(z.y+z.z<0) z.zy = -z.yz; // fold 3	
       z = z*Scale - 1*(Scale-1.0);
       n++;
    }
    return (length(z) ) * pow(Scale, -float(n));
}
*/

float trace(vec3 from, vec3 direction) {
	float totalDistance = 0.0;
	int steps;
	for (steps=0; steps < MaximumRaySteps; steps++) {
		vec3 p = from + totalDistance * direction;
		float distance = DE(p);
		totalDistance += distance;
		if (distance < MinimumDistance) break;
	}
	//return (direction.x / 2 + 0.5);
	return 1.0f-(float(steps) / float(MaximumRaySteps));
}

void FishEyeCamera(vec2 screenPos, float ratio, float fovy, out vec3 direction )
{
    screenPos.y -= 0.2;
    screenPos *= vec2(PI*0.5,PI*0.5/ratio)/fovy;

    direction = vec3(
           sin(screenPos.y+PI*0.5)*sin(screenPos.x)
        , -cos(screenPos.y+PI*0.5)
        , sin(screenPos.y+PI*0.5)*cos(screenPos.x)
    );
}

void main(void)
{
	vec3 from = vec3(0, 0, -2);
	vec3 direction;
	vec2 screenPos = vec2(gl_FragCoord.x/resolution.x - 0.5, gl_FragCoord.y / resolution.y - 0.5);
	vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;

	p.x *= resolution.x/resolution.y;
	direction=normalize(vec3(p.x * 1.4,p.y,1.0));
	float a = trace(from, direction);
	color = vec4(vec3(1, 1, 1) * a, 0.2);
//	color = vec4(1, 1, 1, 0);
}
