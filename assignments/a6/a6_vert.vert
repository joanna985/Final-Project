#version 330 core

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

// You can think of this as a function prototype, it's a way to tell the shader that this function exists.
// What this actually does is it tells the ShaderProgram class to replace this block between the # and # with all the code in perlin.glsl
// This is a way to include other files in your shader.
#
float height(vec2 v);
#

/*uniform variables*/
uniform float iTime;					////time

/*input variables*/
layout (location=0) in vec4 pos;
layout (location=2) in vec4 normal;
layout (location=3) in vec4 uv;
layout (location=4) in vec4 tangent;

/*output variables*/
out vec3 vtx_pos;		////vertex position in the world space

void main()												
{
	vtx_pos = pos.xyz;
	
	#ifdef Terrain
		vtx_pos.z = height(pos.xy);
	#endif

	gl_Position = pvm * vec4(vtx_pos,1);
}