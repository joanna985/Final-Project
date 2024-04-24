#version 330 core

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
in vec2 fragCoord;
out vec4 fragColor;

uniform sampler2D bufferTexture;
uniform sampler2D floor_color;

#define M_PI 3.1415925585
#define Epsilon 1e-6

vec3 sampleDiffuse(int matId, vec3 p);

/////////////////////////////////////////////////////
//// Primitive data types
/////////////////////////////////////////////////////

struct Camera 
{
    vec3 origin;                    /* eye position */
    vec3 horizontal;                /* camera-x axis */
    vec3 vertical;                  /* camera-y axis */
    vec3 LowerLeftCorner;           /* lower-left corner of the image */
};

struct Ray 
{
    vec3 ori;                       /* ray origin */
    vec3 dir;                       /* ray direction */
};

struct Plane 
{
    vec3 n;                         /* plane normal */
    vec3 p;                         /* plane point (a point that is on the plane) */
    int matId;                      /* plane material ID */
};

struct Sphere 
{
    vec3 ori;                       /* sphere origin */
    float r;                        /* sphere radius */
    int matId;                      /* sphere material ID */
};

struct Hit 
{
    float t;                        /* ray parameter t for the intersect */
    vec3 p;                         /* intersect position */
    vec3 normal;                    /* intersect normal */
    int matId;                      /* intersect object's material ID */
};

struct Light 
{
    vec3 position;                  /* light position */
    vec3 Ia;                        /* ambient intensity */
    vec3 Id;                        /* diffuse intensity */
    vec3 Is;                        /* specular intensity */     
};

struct Material 
{
    vec3 ka;                        /* ambient coefficient */
    vec3 kd;                        /* diffuse coefficient */
    vec3 ks;                        /* reflected specular coefficient */
    float shininess;                /* shinness */
    vec3 kr;                        /* reflected coefficient */
};

/////////////////////////////////////////////////////
//// Random functions used in the ray tracer
/////////////////////////////////////////////////////

float g_seed = 0.;

float deg2rad(float deg) 
{
    return deg * M_PI / 180.0;
}

uint base_hash(uvec2 p) 
{
    p = 1103515245U * ((p >> 1U) ^ (p.yx));
    uint h32 = 1103515245U * ((p.x) ^ (p.y >> 3U));
    return h32 ^ (h32 >> 16);
}

void initRand(in vec2 frag_coord, in float time) 
{
    g_seed = float(base_hash(floatBitsToUint(frag_coord))) / float(0xffffffffU) + time;
}

vec2 rand2(inout float seed) 
{
    uint n = base_hash(floatBitsToUint(vec2(seed += .1, seed += .1)));
    uvec2 rz = uvec2(n, n * 48271U);
    return vec2(rz.xy & uvec2(0x7fffffffU)) / float(0x7fffffff);
}

/////////////////////////////////////////////////////
///// Scene objects in the ray tracer
/////////////////////////////////////////////////////

Camera camera;                          //// camera
Light lights[2];                        //// two lights
Sphere spheres[3];                      //// three spheres
Plane planes[1];                        //// one plane
Material materials[4];                  //// four materials: 0 - ground, 1-3 - the three spheres

/////////////////////////////////////////////////////
//// Default declaration of noHit: noHit is returned if no hit is detected
/////////////////////////////////////////////////////

const Hit noHit = Hit(
                 /* time or distance */ -1.0, 
                 /* hit position */     vec3(0), 
                 /* hit normal */       vec3(0), 
                 /* hit material id*/   -1);

/////////////////////////////////////////////////////
//// Default scene setup
/////////////////////////////////////////////////////

void initScene() 
{
    camera = Camera(vec3(0, 15, 50), vec3(5, 0, 0), vec3(0, 3, -3), vec3(-2.5, -1.5, -1));

    // Floor Material 
    materials[0].ka = vec3(0.1);
    materials[0].kd = vec3(0.6);
    materials[0].ks = vec3(0.6);
    materials[0].shininess = 100.0;
    materials[0].kr = 0.5 * materials[0].ks;

    //// Sphere 0 Material
    materials[1].ka = vec3(0.1);
    materials[1].kd = vec3(0.8, 0.2, 0.2);
    materials[1].ks = vec3(0.8);
    materials[1].shininess = 128;
    materials[1].kr = 0.5 * materials[1].ks;

    //// Sphere 1 Material
    materials[2].ka = vec3(0.1);
    materials[2].kd = vec3(0.2, 0.7, 0.2);
    materials[2].ks = vec3(0.8);
    materials[2].shininess = 128;
    materials[2].kr = 0.5 * materials[2].ks;

    //// Sphere 2 Material
    materials[3].ka = vec3(0.2);
    materials[3].kd = vec3(0.2, 0.2, 0.9);
    materials[3].ks = vec3(0.8);
    materials[3].shininess = 128;
    materials[3].kr = 0.5 * materials[3].ks;

    lights[0] = Light(vec3(-1, 3, 0.5), 
                            /*Ia*/ vec3(0.1, 0.1, 0.1), 
                            /*Id*/ vec3(1.0, 1.0, 1.0), 
                            /*Is*/ vec3(0.8, 0.8, 0.8));
    lights[1] = Light(vec3(0.5, 2, 1), 
                            /*Ia*/ vec3(0.1, 0.1, 0.1), 
                            /*Id*/ vec3(0.9, 0.9, 0.9), 
                            /*Is*/ vec3(0.5, 0.5, 0.5));
    planes[0] = Plane(vec3(0, 1, 0), vec3(0, 0, 0), 0);

    spheres[0] = Sphere(vec3(0, 0.6, -1), 0.6, 1);
    spheres[1] = Sphere(vec3(1.1, 0.4, -0.8), 0.4, 2);
    spheres[2] = Sphere(vec3(-1.2, 0.5, -0.8), 0.5, 3);
}

/////////////////////////////////////////////////////
//// Step 1: Ray-plane intersection
//// This function calculates the intersection between a ray and a plane.
//// The idea is to solve the ray parameter t by substituting the ray equation into the plane equation.
//// If t is greater than zero, we will use it to calculate the intersecting point and assemble a Hit object.
//// We provide a default ray-plane intersection implementation for your reference. 
//// Your task is to uncomment the implementaiton, read this function carefully, 
//// and comprehend the implementation details of creating a Hit struct. 
//// You will need to implement this by yourself in the ray-sphere intersection function.
/////////////////////////////////////////////////////

Hit hitPlane(const Ray r, const Plane pl) 
{
    Hit hit = noHit;

    /* default implementation starts */
    
    //// uncomment the following lines and run the code

    // float t = dot(pl.p - r.ori, pl.n) / dot(r.dir, pl.n);

    // if(t <= 0.0) 
    //    return noHit;

    // vec3 hitP = r.ori + t * r.dir;
    // vec3 normal = pl.n;

    // hit = Hit(t, hitP, normal, pl.matId);

    /* default implementation ends */
    
	return hit;
}

/////////////////////////////////////////////////////
//// Step 2: Ray-sphere intersection
//// In this function, you are asked to implement the ray-sphere intersection function
//// Before starting your implementation, read the course slides and the implementation of the hitPlane() function
//// You will calculate the value of A, B, C and check if its discriminant >= 0
//// If so, calculate the smaller solution of t by solving the quadratic equation; Otherwise return noHit
//// If t is greater than zero, use its value to calculate hitP and normal, and assemble hit; Otherwise return noHit
/////////////////////////////////////////////////////

Hit hitSphere(const Ray r, const Sphere s) 
{
    Hit hit = noHit;
	
    /* your implementation starts */

	/* your implementation ends */
    
	return hit;
}

/////////////////////////////////////////////////////
//// Ray-many-object intersection
//// This function calls hitSphere and hitPlane and returns the smallest t as the intersect
//// Read the function carefully to understand its input and output
//// There is no implementation requirement for this function; but you will need to call this function in your future implementation
/////////////////////////////////////////////////////

Hit findHit(Ray r) 
{
    Hit h = noHit;
	
	for(int i = 0; i < spheres.length(); i++) {
        Hit tempH = hitSphere(r, spheres[i]);
        if(tempH.t > Epsilon && (h.t < 0. || h.t > tempH.t))
            h = tempH;
    }
	
    for(int i = 0; i < planes.length(); i++) {
        Hit tempH = hitPlane(r, planes[i]);
        if(tempH.t > Epsilon && (h.t < 0. || h.t > tempH.t))
            h = tempH;
    }

    return h;
}

/////////////////////////////////////////////////////
//// Step 3: Phong shading
//// In this function, you will implement the Phong shading model to be used in your ray tracer
//// It is the standard version we have practiced in our previous assignments, 
//// You are allowed to reuse the code you have implemented previously
/////////////////////////////////////////////////////

vec3 shadingPhong(Light light, int matId, vec3 e, vec3 p, vec3 s, vec3 n) 
{
	//// default color: return dark red for the ground and dark blue for spheres
    vec3 color = matId == 0 ? vec3(0.2, 0, 0) : vec3(0, 0, 0.3);

    vec3 ka = materials[matId].ka;
    vec3 kd = materials[matId].kd;
    vec3 ks = materials[matId].ks;
    float shininess = materials[matId].shininess;
    
    /* your implementation starts */
	
	/* your implementation ends */
    
	return color;
}

/////////////////////////////////////////////////////
//// Step 4: Texture mapping
//// In this function, you are asked to (1) read the texture color from the texture sampler floor_cloor with the given uv, 
//// and (2) multiply the texture color with its material color and assign the multiplication value to the returned color.
/////////////////////////////////////////////////////

vec3 sampleDiffuse(int matId, vec3 p) 
{
    vec3 mat_color = materials[matId].kd;   /* material color */
    vec3 color = mat_color;                 /* color to be returned */

    /* apply texture for the ground */
    if(matId == 0) {		
		vec2 uv = vec2(p.x, p.z) / 5.0;     /* uv texture on the ground */

        /* your implementation starts */
        
        
		/* your implementation ends */
    }

    /* no texture for the spheres */
    
    return color;
}

/////////////////////////////////////////////////////
//// Step 5: Shadow
//// Your task is to create a shadow ray pointing from the intersect to the light source
//// and then check if the shadow ray interests with objects in the scene by calling the findHit() function
//// Only a t value that is between zero and t_max indicates a shadow; otherwise no shadow
//// You may want to offset the origin of the shadow ray along its direction by a small number 
//// (e.g., using the Epsilon we deleared in the shader) to avoid any self-intersection
/////////////////////////////////////////////////////

bool isShadowed(Light light, Hit h) 
{
    bool shadowed = false;                          /* returned boolean to indicate if there is shadow or not */
	vec3 intersect = h.p;                           /* hit intersect */
	vec3 toLight = light.position-intersect;        /* vector pointing from the shadow ray origin to the light */
	float t_max = length(toLight);                  /* length of toLight */
	vec3 dir = normalize(toLight);                  /* direction of toLight */
	
    /* your implementation starts */
	
    
	/* your implementation ends */
    
	return shadowed;
}

/////////////////////////////////////////////////////
//// Generate the primary ray using the input screen coordinates
//// No implementation is required for this function
/////////////////////////////////////////////////////

Ray getPrimaryRay(vec2 uv) 
{
    return Ray(camera.origin, camera.LowerLeftCorner + uv.x * camera.horizontal + uv.y * camera.vertical - camera.origin);
}

/////////////////////////////////////////////////////
//// Calculate the ray-object interesction in the scene and return the pixel color
//// The intersection will be recorded in the hit variable
//// No implementation is required for this function
/////////////////////////////////////////////////////

vec3 rayTrace(in Ray r, out Hit hit) 
{
    vec3 col = vec3(0);
    Hit h = findHit(r);
    hit = h;
    if(h.t > 0 && h.t < 1e8) {
        // shading
        for(int i = 0; i < lights.length(); i++) {
            if(isShadowed(lights[i], h)) {
                col += materials[h.matId].ka * lights[i].Ia;
            } else {
                vec3 e = camera.origin;
                vec3 p = h.p;
                vec3 s = lights[i].position;
                vec3 n = h.normal;
                col += shadingPhong(lights[i], h.matId, e, p, s, n);
            }
        }
    }
    return col;
}

/////////////////////////////////////////////////////
//// Step 6.I: Change the value of recursiveDepth from 1 to 50
//// By default this value was set to be 1 for basic (non-recursive) ray tracing 
/////////////////////////////////////////////////////

/* your implementation starts */

const int recursiveDepth = 1;

/* your implementation ends */


void main() 
{
    initScene();
    initRand(fragCoord, iTime);             
    vec2 uv = fragCoord / iResolution.xy;
    vec3 compounded_kr = vec3(1.0);     /* cumulative reflection coefficient */


    vec3 resultCol = vec3(0);
    Ray recursiveRay = getPrimaryRay(uv + rand2(g_seed) / iResolution.xy);
	
    for(int i = 0; i < recursiveDepth; i++) {
        Hit hit;
        vec3 col = rayTrace(recursiveRay, hit);

        resultCol += compounded_kr * col;

        if(hit.t < 0.0 || hit.t > 1e8 || length(compounded_kr) < 0.001)
            break;

        compounded_kr *= materials[hit.matId].kr;
		
        /////////////////////////////////////////////////////
        //// Step 6.II: Calculate the reflected ray and assign it to recursiveRay for the next loop
        //// You are asked to calculate the reflected ray based on the current ray direction and intersect
        //// You need to (1) calculate the reflected ray direction (store the value in relfected_dir) and 
        //// (2) use this reflected direction along with the intersecting point to calculate the recursive ray for the next loop
        /////////////////////////////////////////////////////
        
        vec3 intersect = hit.p;
        vec3 normal = hit.normal;
        vec3 incoming_dir = recursiveRay.dir;
        vec3 reflected_dir = vec3(0);           /* calculate the reflected dir */

		/* your implementation starts */
        
		/* your implementation ends */
    }
	
    // Temporal AA
    fragColor = vec4((resultCol + float(iFrame - 1) * texture(bufferTexture, uv).xyz) / float(iFrame), 1.);
}