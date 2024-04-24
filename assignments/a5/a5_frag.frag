#version 330 core

/*default camera matrices. do not modify.*/
layout(std140) uniform camera {
    mat4 projection;	/*camera's projection matrix*/
    mat4 view;			/*camera's view matrix*/
    mat4 pvm;			/*camera's projection*view*model matrix*/
    mat4 ortho;			/*camera's ortho projection matrix*/
    vec4 position;		/*camera's position in world space*/
};

/*input variables*/
in vec3 vtx_normal;             /* vtx normal vector in world space */
in vec3 vtx_position;           /* vtx position in world space */
in vec3 vtx_model_position;     /* vtx position in model space */
in vec4 vtx_color;              /* vtx color */
in vec2 vtx_uv;                 /* vtx uv coordinates */
in vec3 vtx_tangent;            /* vtx tangent vector in world space */

uniform vec3 ka;                /* object material ambient */
uniform vec3 kd;                /* object material diffuse */
uniform vec3 ks;                /* object material specular */
uniform float shininess;        /* object material shininess */

uniform sampler2D tex_color;    /* texture sampler for color */
uniform sampler2D tex_normal;   /* texture sampler for normal vector */

/*output variables*/
out vec4 frag_color;

struct Light 
{
    vec3 position;          /* light position */
    vec3 Ia;                /* ambient intensity */
    vec3 Id;                /* diffuse intensity */
    vec3 Is;                /* specular intensity */     
};

//// Declare three light sources

const Light light1 = Light(/*position*/ vec3(3, 1, 3), 
                            /*Ia*/ vec3(0.1, 0.1, 0.1), 
                            /*Id*/ vec3(1.0, 1.0, 1.0), 
                            /*Is*/ vec3(0.5, 0.5, 0.5));
const Light light2 = Light(/*position*/ vec3(0, 0, -5), 
                            /*Ia*/ vec3(0.1, 0.1, 0.1), 
                            /*Id*/ vec3(0.9, 0.9, 0.9), 
                            /*Is*/ vec3(0.5, 0.5, 0.5)); 
const Light light3 = Light(/*position*/ vec3(-5, 1, 3), 
                            /*Ia*/ vec3(0.1, 0.1, 0.1), 
                            /*Id*/ vec3(0.9, 0.9, 0.9), 
                            /*Is*/ vec3(0.5, 0.5, 0.5));

/////////////////////////////////////////////////////
//// Step 1 function: Visualize UV Coordinates with Checkerboard
//// In this function, you will implement the checkerboard visualization of uv mapping
//// Your task is to calculate the value of color using the value of uv to render a checkerboard on the mesh surface
//// We provide a reference figure in the assignment document; but you are free to choose your own grid size
/////////////////////////////////////////////////////

vec4 shading_texture_with_checkerboard() 
{
    vec3 color = vec3(0.0);     //// we set the default color to be black, update its value in your implementation below
    vec2 uv = vtx_uv;           //// the uv coordinates you need to calculate the checkerboard color

    /* your implementation starts */
    

    /* your implementation ends */

    return vec4(color, 1.0);
}

/////////////////////////////////////////////////////
//// Step 2 Function: Read Color from Texture Sampler
//// In this function, you will practice reading texture value from texture sampler using uv coordinates
//// Your task is to read texture value from the texture sampler named tex_color with uv,
//// and then assign this texture value to the output color
/////////////////////////////////////////////////////

vec4 shading_texture_with_color() 
{
    vec4 color = vec4(0.0);     //// we set the default color to be black, update its value in your implementation below
    vec2 uv = vtx_uv;           //// the uv coordinates you need to read texture values

    /* your implementation starts */
    

    /* your implementation ends */

    return color;
}

/////////////////////////////////////////////////////
//// Step 3 Function: Phong Shading with Texture
//// In this function, you will incorporate the texture color into the Phong shading model
//// Your task is to use the texture color as a multiplier in the diffuse term of the Phong model
//// i.e., diff_color = kd * Id * max(0, dot(n,l)) * tex_color
//// The definitions of the input variables are the same of your shading_phong function in A4 (see below)
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//// input variables for shading_texture_with_phong
/////////////////////////////////////////////////////
//// light: the light struct
//// e: eye position
//// p: position of the point
//// s: light source position (you may also use light.position)
//// n: normal at the point
/////////////////////////////////////////////////////

vec4 shading_texture_with_phong(Light light, vec3 e, vec3 p, vec3 s, vec3 n) 
{
    vec4 color = vec4(0.0);                                 //// we set the default color to be black, update its value in your implementation below
    vec3 tex_color = shading_texture_with_color().rgb;      //// the texture value read from your previously implemented function; you need to use this value in your phong shading model
    
    /* your implementation starts */
    

    /* your implementation ends */

    return color;
}

//// This function calls your shading_texture_with_phong function with the three declared light sources
//// No implementation requirement for this function
vec4 shading_texture_with_lighting()
{
    vec3 e = position.xyz;              //// eye position
    vec3 p = vtx_position;              //// surface position
    vec3 n = normalize(vtx_normal);     //// normal vector

    vec4 color = shading_texture_with_phong(light1, e, p, light1.position, n) 
               + shading_texture_with_phong(light2, e, p, light2.position, n)
               + shading_texture_with_phong(light3, e, p, light3.position, n);
    return color;
}

/////////////////////////////////////////////////////
//// Step 4 Function: Phong Shading with Normal Mapping
//// In this function, you will implement normal mapping within the Phong shading model
//// Your tasks include implementing four functions (Part 1-4) and then using these functions to calculate the perturbed normal (Part 5)
//// The perturbed normal will be used in your previously implemented shading_texture_with_phong() function to calculate the output color
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
//// Step 4 - Part 1: Calculate the Bitangent Normal
//// In this function, you will calculate the bitangent normal vector as the cross product between the normal vector and the tangent vector
//// Make sure you normalize the vector before returning it
/////////////////////////////////////////////////////

vec3 calc_bitangent(vec3 N, vec3 T) 
{
    vec3 B = vec3(0.0);     //// the bitangent vector you need to calculate

    /* your implementation starts */
    

    /* your implementation ends */
    
    return B;
}

/////////////////////////////////////////////////////
//// Step 4 - Part 2: Calculate the TBN Matrix
//// In this function, you will assemble the TBN matrix using the T, B, N vectors, with each vector as a column of the matrix
/////////////////////////////////////////////////////

mat3 calc_TBN_matrix(vec3 T, vec3 B, vec3 N) 
{
    mat3 TBN = mat3(0.0);   //// the TBN matrix you need to calculate

    /* your implementation starts */


    /* your implementation ends */

    return TBN;
}

/////////////////////////////////////////////////////
//// Step 4 - Part 3: Read Normal Coordinates from the Normal Texture
//// In this function, you have two sub-tasks: 
//// (1) read a vec3 from tex_normal with uv; 
//// (2) map the range of each component of the read vector from [0,1] to [-1,1], and then return the remapped vector
//// Keep in mind that the texture() function returns a vec4 by default.
/////////////////////////////////////////////////////

vec3 read_normal_texture() 
{
    vec3 normal = vec3(0.0);    //// the normal vector you need to update
    vec2 uv = vtx_uv;           //// the uv coordinates you need to 

    /* your implementation starts */
    

    /* your implementation ends */

    return normal;
}

/////////////////////////////////////////////////////
//// Step 4 - Part 4: Calculate the Perturbed Normal with TBN Matrix and the Texture Normal 
//// In this function, you will multiply the TBN matrix with the remapped normal read from the normal texture to get the perturbed normal
/////////////////////////////////////////////////////

vec3 calc_perturbed_normal(mat3 TBN, vec3 normal) 
{
    vec3 perturbed_normal = vec3(0.0);
    
    /* your implementation starts */


    /* your implementation ends */
    
    return perturbed_normal;
}

/////////////////////////////////////////////////////
//// Step 4 - Part 5: Phong Shading with Normal Mapping
//// In this function, you will put together the functions you have implemented above to calculate the perturbed normal with TBN matrix
//// Your tasks include: 
//// (1) calculate B; 
//// (2) assemble TBN with T, B, N; 
//// (3) read normal from texture (and remap each of its component to [-1,1]);
//// (4) calculate the perturbed normal with TBN and the read normal.
//// The perturbed normal will then be used in the shading_texture_with_phong() function to calculate the Phong shading color with the three point lights.
/////////////////////////////////////////////////////

vec4 shading_texture_with_normal_mapping()
{
    vec3 e = position.xyz;              //// eye position
    vec3 p = vtx_position;              //// surface position

    vec3 N = normalize(vtx_normal);     //// normal vector
    vec3 T = normalize(vtx_tangent);    //// tangent vector

    vec3 perturbed_normal = vec3(0.0);  //// perturbed normal

    /* your implementation starts */
    

    /* your implementation ends */

    vec4 color = shading_texture_with_phong(light1, e, p, light1.position, perturbed_normal)
               + shading_texture_with_phong(light2, e, p, light2.position, perturbed_normal)
               + shading_texture_with_phong(light3, e, p, light3.position, perturbed_normal);
    return color;
}

void main() 
{
    //// Step 1: Visualize UV Coordinates with Checkerboard
    //// Your task is to implement the shading_texture_with_checkerboard() function
    frag_color = shading_texture_with_checkerboard();

    //// Step 2: Read Color from Texture Sampler
    //// Your task is to implement the shading_texture_with_color() function
    //// Uncomment the following line to call the function (you might also need to comment out previous lines that assign frag_color)
    // frag_color = shading_texture_with_color();

    //// Step 3: Phong Shading with Texture
    //// Your task is to implement the shading *shading_texture_with_phong()* function, that is called within shading_texture_with_lighting()
    //// Uncomment the following line to call the function (you might also need to comment out previous lines that assign frag_color)
    // frag_color = shading_texture_with_lighting();

    //// Step 4: Phong Shading with Normal Mapping
    //// Your tasks are to implement the five functions as mentioned below that are used to calcuate perturbed normal vector in the shading_texture_with_normal_mapping() function: 
    //// (1) calc_bitangent(), (2) calc_TBN_matrix(), (3) read_normal_texture(), (4) calc_perturbed_normal(), and (5) shading_texture_with_normal_mapping()
    //// Uncomment the following line to call the function (you might also need to comment out previous lines that assign frag_color)
    // frag_color = shading_texture_with_normal_mapping();
}