#version 330 core

layout (std140) uniform camera
{
	mat4 projection;
	mat4 view;
	mat4 pvm;
	mat4 ortho;
	vec4 position;
};

/*uniform variables*/
uniform float iTime;

/*input variables*/
in vec3 vtx_pos;

/*input variables*/
out vec4 frag_color;

//// You can think of this as a function prototype, it's a way to tell the shader that this function exists.
//// What this actually does is it tells the ShaderProgram class to replace this block between the # and # with all the code in perlin.glsl
//// This is a way to include other files in your shader.
#
vec3 shading_noise(vec3 vtx_pos);
vec3 shading_terrain(vec3 vtx_pos);
#

void main()
{
	#ifdef Terrain
        frag_color = vec4(shading_terrain(vtx_pos), 1.0);
    #else
        frag_color = vec4(shading_noise(vtx_pos), 1.0);
    #endif
}
