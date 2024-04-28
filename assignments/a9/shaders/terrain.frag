#version 330 core

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

/* set light ubo. do not modify.*/
struct light
{
	ivec4 att; 
	vec4 pos; // position
	vec4 dir;
	vec4 amb; // ambient intensity
	vec4 dif; // diffuse intensity
	vec4 spec; // specular intensity
	vec4 atten;
	vec4 r;
};
layout(std140) uniform lights
{
	vec4 amb;
	ivec4 lt_att; // lt_att[0] = number of lights
	light lt[4];
};

uniform float iTime;
uniform mat4 model;		/*model matrix*/

in vec3 vtx_pos;

out vec4 frag_color;


uniform vec3 ka;            /* object material ambient */
uniform vec3 kd;            /* object material diffuse */
uniform vec3 ks;            /* object material specular */
uniform float shininess;    /* object material shininess */

/*vec2 hash2(vec2 v) {
	vec2 rand = vec2(0,0);
	
	//rand  = 50.0 * 1.05 * fract(v * 0.3183099 + vec2(0.71, 0.113));
    //rand = -1.0 + 2 * 1.05 * fract(rand.x * rand.y * (rand.x + rand.y) * rand);

	rand  = 52.5 * fract(v.yx * 0.31 + vec2(0.31, 0.113));
    rand = -1.0 + 3.1 * fract(rand.x * rand.y * rand.yx);
	return rand;
}*/


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
	/*if (v.x < -10.0) {
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
    vec2 m = f*f*(3.0-2.0*f);
	
	noise = mix( mix( dot( hash2(i + vec2(0.0, 0.0)), f - vec2(0.0,0.0)),
					 dot( hash2(i + vec2(1.0, 0.0)), f - vec2(1.0,0.0)), m.x),
				mix( dot( hash2(i + vec2(0.0, 1.0)), f - vec2(0.0,1.0)),
					 dot( hash2(i + vec2(1.0, 1.0)), f - vec2(1.0,1.0)), m.x), m.y);
	return noise;
}



float noiseOctave(vec2 v, int num) {
	float sum = 0;
	for(int i =0; i<num; i++){
		sum += pow(2,-1*i) * perlin_noise(pow(2,i) * v);
	}
	return sum;
}



float height(vec2 v) {
    float h = 0;
	h = 0.75 * noiseOctave(v, 10);
	if(h<0) h *= .5;
	return h * 2.;
}


vec3 compute_normal(vec2 v, float d) {	
	vec3 normal_vector = vec3(0,0,0);
	vec3 v1 = vec3(v.x + d, v.y, height(vec2(v.x + d, v.y)));
	vec3 v2 = vec3(v.x - d, v.y, height(vec2(v.x - d, v.y)));
	vec3 v3 = vec3(v.x, v.y + d, height(vec2(v.x, v.y + d)));
	vec3 v4 = vec3(v.x, v.y - d, height(vec2(v.x, v.y - d)));
	
	normal_vector = normalize(cross(v1-v2, v3-v4));
	return normal_vector;
}


vec4 shading_phong(light li, vec3 e, vec3 p, vec3 s, vec3 n) {
    vec3 v = normalize(e - p);
    vec3 l = normalize(s - p);
    vec3 r = normalize(reflect(-l, n));

    vec3 ambColor = ka * li.amb.rgb;
    vec3 difColor = kd * li.dif.rgb * max(0., dot(n, l));
    vec3 specColor = ks * li.spec.rgb * pow(max(dot(v, r), 0.), shininess);

    return vec4(ambColor + difColor + specColor, 1);
}



// Color the terrain
/*vec3 shading_terrain(vec3 pos) {
    vec3 n = compute_normal(pos.xy, 0.01);
    vec3 e = position.xyz;
    vec3 p = pos.xyz;
    vec3 s = lt[0].pos.xyz;
    n = normalize((model * vec4(n, 0)).xyz);
    p = (model * vec4(p, 1)).xyz;

    // Compute ambient, diffuse, and specular components using Phong shading
    vec3 phongColor = shading_phong(lt[0], e, p, s, n).xyz;

    // Map height to color
    float h = pos.z + 0.8;
    h = clamp(h, 0.0, 1.0);
    
    // Adjust the range of height values to map to the desired color gradient
    vec3 color = mix(vec3(0.0, 0.0, 1.0), vec3(0.7, 0.7, 1.0), h);

    // Make the points of highest elevation slightly lighter
    if (h == 1.0) {
        color += 0.1; // Increase brightness slightly
    }

    // Combine Phong shading with height-based color
    vec3 finalColor = phongColor * color;

    return finalColor;
}*/



vec3 shading_grass(vec3 pos) {
	vec3 n = compute_normal(pos.xy, 0.01);
    vec3 e = position.xyz;
    vec3 p = pos.xyz;
    vec3 s = lt[0].pos.xyz;
    n = normalize((model * vec4(n, 0)).xyz);
    p = (model * vec4(p, 1)).xyz;

    // Compute ambient, diffuse, and specular components using Phong shading
    vec3 phongColor = shading_phong(lt[0], e, p, s, n).xyz;

    // Map height to color
    float h = pos.z + 0.8;
    h = clamp(h, 0.0, .5);
    
    // Adjust the range of height values to map to the desired color gradient
    vec3 color = mix(vec3(0.0, 0.2, 0.0), vec3(0.0, 0.2, 0.0), h);

    // Make the points of highest elevation slightly lighter
    if (h == .5) {
        color += .5; // Increase brightness slightly
    }

    // Combine Phong shading with height-based color
    vec3 finalColor = phongColor * color;

    return finalColor;
}


void main() {
    frag_color = vec4(shading_grass(vtx_pos), 1.0);
}
