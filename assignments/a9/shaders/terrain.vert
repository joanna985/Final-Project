#version 330 core

layout(std140) uniform camera
{
    mat4 projection;
    mat4 view;
    mat4 pvm;
    mat4 ortho;
    vec4 position;
};

vec2 hash2(vec2 v) {
    vec2 rand = vec2(1, 1);

    // Your implementation of hash function
    //rand = 50.0 * 1.05 * fract(v * 0.3183099 + vec2(0.71, 0.113));
    //rand = -1.0 + 2 * 2.05 * fract(rand.x * rand.y * (rand.x + rand.y) * rand);
    //vec2 rand = vec2(0.0);
    rand.x = fract(sin(dot(v, vec2(12.9898, 78.233))) * 43758.5453);
    rand.y = fract(sin(dot(v, vec2(35.6897, 41.7279))) * 269.6942);

    /*if (v.y > ((v.x * v.x) - 10.0)) {
        rand.x = fract(sin(dot(v, vec2(12.9898, 78.233))) * 43758.5453);
        rand.y = fract(sin(dot(v, vec2(35.6897, 41.7279))) * 269.6942);

    } else if (v.y < (-v.x - 15.0)) {
		rand = 50.0 * 1.05 * fract(v * 0.3183099 + vec2(0.71, 0.113));
    	rand = -1.0 + 2 * 2.05 * fract(rand.x * rand.y * (rand.x + rand.y) * rand);
		
	} else {
		rand = vec2(1, 1);
	}*/

    /*if (v.y > 2.0) {
        rand.x = fract(sin(dot(v, vec2(12.9898, 78.233))) * 43758.5453);
        rand.y = fract(sin(dot(v, vec2(35.6897, 41.7279))) * 269.6942);
    } else {
        rand = vec2(1, 1);
    }*/

    return rand;
    //return rand;
}








float perlin_noise(vec2 v) {
    float noise = 0;
    vec2 i = floor(v);
    vec2 f = fract(v);
    vec2 m = f * f * (3.0 - 2.0 * f);
    noise = mix(mix(dot(hash2(i + vec2(0.0, 0.0)), f - vec2(0.0, 0.0)), dot(hash2(i + vec2(1.0, 0.0)), f - vec2(1.0, 0.0)), m.x), mix(dot(hash2(i + vec2(0.0, 1.0)), f - vec2(0.0, 1.0)), dot(hash2(i + vec2(1.0, 1.0)), f - vec2(1.0, 1.0)), m.x), m.y);
    return noise;
}

float noiseOctave(vec2 v, int num) {
    float sum = 0;
    for(int i = 0; i < num; i++)
    {
        sum += pow(2, -1 * i) * perlin_noise(pow(2, i) * v);
    }
    return sum;
}

float height(vec2 v) {
    float h = 0;
    h = 0.15 * noiseOctave(v, 10); // Adjust the parameters for the water surface
    if(h < 0)
        h *= .5;
    return h * 2.;
}

uniform mat4 model;      /*model matrix*/

/*input variables*/
layout(location = 0) in vec4 pos;

/*output variables*/
out vec3 vtx_pos;       //vertex position in the world space



void main()
{
    vtx_pos = pos.xyz;
    vtx_pos.z = height(pos.xy); // Set the vertex position's Z coordinate to the height of the water surface

    gl_Position = pvm * model * vec4(vtx_pos, 1.);
}
