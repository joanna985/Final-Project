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

/*input variables*/
in vec3 vtx_normal; // vtx normal in world space
in vec3 vtx_position; // vtx position in world space
in vec3 vtx_model_position; // vtx position in model space
in vec4 vtx_color;
in vec2 vtx_uv;
in vec3 vtx_tangent;

uniform vec3 ka;            /* object material ambient */
uniform vec3 kd;            /* object material diffuse */
uniform vec3 ks;            /* object material specular */
uniform float shininess;    /* object material shininess */

uniform sampler2D tex_color;   /* texture sampler for color */
uniform sampler2D tex_normal;   /* texture sampler for normal vector */

/*output variables*/
out vec4 frag_color;

vec4 shading_texture_with_color() 
{
    vec4 color = vec4(0.0);     //// we set the default color to be black, update its value in your implementation below
    vec2 uv = vtx_uv;           //// the uv coordinates you need to read texture values

    /* your implementation starts */
    color = texture(tex_color, uv);

    /* your implementation ends */

    return color;
}

vec3 shading_texture_with_phong(light li, vec3 e, vec3 p, vec3 s, vec3 n)
{
    vec3 color = vec3(0.0);
    // vec3 l = normalize(s-p);
    // float result = 0.0;
    // for (int i = 0; i < 3; i++) {
    //     result += l[i]*n[i];
    // }
    // float max1 = 0.0;
    // if(0.0 < result) {
    //     max1 = result;
    // }

    // vec3 v = normalize(e-p);
    // vec3 r = normalize(reflect(-l,n));
    // float result2 = 0.0;
    // for (int i = 0; i < 3; i++) {
    //     result2 += v[i]*r[i];
    // }
    // float max2 = 0.0;
    // if(0.0 < result2) {
    //     max2 = result2;
    // }

    vec3 textColor = shading_texture_with_color().rgb;
    color = ka * vec3(li.amb.x, li.amb.y, li.amb.z) + kd * vec3(li.dif.x, li.dif.y, li.dif.z) * max(0, dot(n, normalize(s - p))) * textColor + ks * vec3(li.spec.x, li.spec.y, li.spec.z) * pow(max(0, dot(normalize(s - e), reflect(normalize(s - p), n))), shininess);
    


    /* your implementation ends */

    return color;
}

vec3 read_normal_texture()
{
    vec3 normal = texture(tex_normal, vtx_uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    return normal;
}

void main()
{
    vec3 e = position.xyz;              //// eye position
    vec3 p = vtx_position;              //// surface position
    vec3 N = normalize(vtx_normal);     //// normal vector
    vec3 T = normalize(vtx_tangent);    //// tangent vector

    vec3 texture_normal = read_normal_texture();
    vec3 texture_color = texture(tex_color, vtx_uv).rgb;

    frag_color = vec4(mix(mix(shading_texture_with_phong(lt[0], e, p, vec3(lt[0].pos.x, lt[0].pos.y, lt[0].pos.z), N), shading_texture_with_phong(lt[1], e, p, vec3(lt[1].pos.x, lt[1].pos.y, lt[1].pos.z), N), .8), mix(shading_texture_with_phong(lt[2], e, p, vec3(lt[2].pos.x, lt[2].pos.y, lt[2].pos.z), N), shading_texture_with_phong(lt[3], e, p, vec3(lt[3].pos.x, lt[3].pos.y, lt[3].pos.z), N), .8), .8), 1.0);
}