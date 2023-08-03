#ifndef LIGHTSTRUCT
#define LIGHTSTRUCT

struct Light {
	vec3	position;
	float	radius;
	vec3	intensity;
	float	unused;
};

#endif