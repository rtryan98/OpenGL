#version 460 core

#define MAX_STEPS 128
#define FAR 1000.
#define HIT .01
#define EPSILON .001

layout(rgba32f) writeonly uniform image2D uRaycast;

struct sRay
{
    vec3 origin;
    vec3 direction;
};

uniform struct sCamera
{
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
} camera;

uniform struct sScreen
{
    float fovX;
    float fovY;
} screen;

uniform float power = 2.0;

vec3 lightDir = normalize(vec3(0.5, -1.0, 0.5));
vec3 colorAMix = vec3(0.40, 0.0, 0.65);
vec3 colorBMix = vec3(0.65, 0.15, 0.25);

// http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-v-the-mandelbulb-different-de-approximations/
// Modified version from blog + Sebastian Lague https://github.com/SebLague
vec2 mandelbulb(vec3 position)
{
	vec3 z = position;
	float dr = 1.0;
	float r = 0.0;
	uint iteration = 0;
	for(uint i = 0; i < 15; i++)
	{
		iteration = i;
		r = length(z);
		if(r > 2.0)
		{
			break;
		}
		float theta = acos(z.z/r);
		float phi = atan(z.x, z.y);
		dr = pow(r, power - 1.0) * power * dr + 1.0;
		float zr = pow(r, power);
		theta = theta * power;
		phi = phi * power;
		z = zr * vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=position;
	}
	float dst = 0.5 * log(r)*r/dr;
	return vec2(iteration, dst * 1.0);
}

sRay initRay()
{
    ivec2 dims = imageSize(uRaycast);
    float halfWidth = (float(dims.x) / 2.0);
    float halfHeight = (float(dims.y) / 2.0);

    float pixelNDCX = screen.fovX * (float(gl_GlobalInvocationID.x) + 0.5 - halfWidth) / halfWidth;
    float pixelNDCY = screen.fovY * (float(gl_GlobalInvocationID.y) + 0.5 - halfHeight) / halfHeight;

    vec3 dir = normalize(pixelNDCX * camera.right + pixelNDCY * camera.up + camera.front);

    sRay result;
    result.origin = camera.position;
    result.direction = dir;
    return result;
}

vec3 estimateNormal(vec3 point)
{
	float x = mandelbulb(vec3(point.x + EPSILON, point.y, point.z)).y - mandelbulb(vec3(point.x - EPSILON, point.y, point.z)).y;
	float y = mandelbulb(vec3(point.x, point.y + EPSILON, point.z)).y - mandelbulb(vec3(point.x, point.y - EPSILON, point.z)).y;
	float z = mandelbulb(vec3(point.x, point.y, point.z + EPSILON)).y - mandelbulb(vec3(point.x, point.y, point.z - EPSILON)).y;
	return normalize(vec3(x, y, z));
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
	if(gl_GlobalInvocationID.x > imageSize(uRaycast).x || gl_GlobalInvocationID.y > imageSize(uRaycast).y)
	{
		return;
	}
    sRay ray = initRay();
    vec4 result = vec4(0.0);

	float lastDist = 0.0;
	vec3 currentPoint = ray.origin;
	uint steps = 0;
	for(uint i = 0; i < MAX_STEPS; i++)
	{
		vec2 bulb = mandelbulb(currentPoint);
		float dist = bulb.y;
		if(dist <= EPSILON)
		{
			float escapeIterations = bulb.x;
			vec3 normal = estimateNormal(currentPoint-ray.direction*EPSILON*2);
			float colorA = clamp(dot(normal * 0.5 + 0.5, -lightDir), 0.0, 1.0);
			float colorB = clamp(float(escapeIterations) / 16.0, 0.0, 1.0);
			vec3 color = clamp(vec3(colorA * colorAMix + colorB * colorBMix), 0.0, 1.0);
			result = vec4(color, 1.0);
			break;
		}
		lastDist += dist;
		currentPoint += dist * ray.direction;
		steps++;
	}
	float rim = float(steps) / 25.0;
    imageStore(uRaycast, ivec2(gl_GlobalInvocationID.xy), result * rim);
}
