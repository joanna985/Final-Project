#version 330 core

/*default camera matrices. do not modify.*/
layout(std140) uniform camera {
	mat4 projection;	/*camera's projection matrix*/
	mat4 view;			/*camera's view matrix*/
	mat4 pvm;			/*camera's projection*view*model matrix*/
	mat4 ortho;			/*camera's ortho projection matrix*/
	vec4 position;		/*camera's position in world space*/
};

uniform mat4 model;				/*model transform matrix*/

/*input variables*/
layout(location = 0) in vec4 pos;			/*vertex position*/
layout(location = 1) in vec4 v_color;		/*vertex color*/
layout(location = 2) in vec4 normal;			/*vertex normal*/

/*output variables*/
out vec4 vtx_color;
out vec3 vtx_normal; // world space normal	
out vec3 vtx_position; // world space position

void main() {
	vec4 worldNormal = model * vec4(normal.xyz, 0.);
	vec4 worldPos = model * vec4(pos.xyz, 1.);

	vtx_normal = normalize(worldNormal.xyz);
	vtx_position = worldPos.xyz;
	vtx_color = vec4(v_color.rgb, 1.);

	gl_Position = pvm * worldPos;
}