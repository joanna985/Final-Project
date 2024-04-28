#version 330 core

/*default camera matrices. do not modify.*/
layout(std140) uniform camera
{
    mat4 projection;	/*camera's projection matrix*/
    mat4 view;			/*camera's view matrix*/
    mat4 pvm;			/*camera's projection*view*model matrix*/
    mat4 ortho;			/*camera's ortho projection matrix*/
    vec4 position;		/*camera's position in world space*/
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

uniform samplerCube skybox;

/*input variables*/
in vec3 vtx_normal; // vtx normal in world space
in vec3 vtx_position; // vtx position in world space
in vec3 vtx_model_position; // vtx position in model space
in vec4 vtx_color;
in vec2 vtx_uv;
in vec3 vtx_tangent;

/*output variables*/
out vec4 frag_color;




void main()
{

	// Initialize final color with ambient light
    vec3 finalColor = lights.amb.rgb;

    // Iterate over all lights
    for (int i = 0; i < lights.lt_att.x; ++i) {
        // Calculate light direction and distance
        vec3 lightDir = normalize(lights.lt[i].pos.xyz - vtx_position);
        float distanceToLight = length(lights.lt[i].pos.xyz - vtx_position);

        // Calculate diffuse and specular contributions
        float diffuseFactor = max(dot(lightDir, normalize(vtx_normal)), 0.0);
        vec3 diffuse = lights.lt[i].dif.rgb * diffuseFactor;

        // Add diffuse contribution to final color
        finalColor += diffuse;

        // Calculate specular contribution (you need to implement this)

        // Apply attenuation (you need to implement this)

        // Apply any other lighting effects here

        // Optionally, break the loop if you reached the maximum number of lights
        if (i >= 3) break;
    }

    //vec3 color = shading_texture_with_checkerboard();
    vec3 I = normalize(position.xyz - vtx_position);
    vec3 R = reflect(I, normalize(vtx_normal));
    vec3 color = texture(skybox, vec3(R.x, -R.y, -R.z)).rgb;
    frag_color = vec4(color, 1.0);
}