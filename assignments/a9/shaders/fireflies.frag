#version 330 core

uniform vec2 iResolution;       /* window resolution */
uniform float iTime;            /* time */
uniform int iFrame;             /* frame index */
in vec2 fragCoord;              /* screen space coordinate */
out vec4 outputColor;           /* output color */

/////////////////////////////////////////////////////
//// Macro definition for time: this is the actual time you use for your animation
/////////////////////////////////////////////////////
//// You may adjust the multipler below to make the animaiton run fast (by increasing the value) 
//// or run slower (by decreasing the value), according to your needs (it varies on different laptops).
//// We set the default value to be 1.0.
/////////////////////////////////////////////////////

#define Time (iTime*1.0)            

#define PI 3.14159265359
#define TWO_PI 6.28318530718
#define Gravity 0.7             /* gravity */
#define NUM_FIREFLIES 25        /* number of fireworks */
#define DURATION 3.             /* duration of each fireworks period */

const vec2 g = vec2(.0, -Gravity); /* gravity */


/////////////////////////////////////////////////////
//// Hash functions
/////////////////////////////////////////////////////

//// This hash function takes input t and returns random float between 0 and 1
float hash1d(float t)
{
    t += 1.;
    return fract(sin(t * 674.3) * 453.2);
}



vec2 randMov(vec2 p) {
    // vec3 a = fract(vec3(p.xyx) * vec3(213.897, 653.453, 253.098));
    // a += dot(a, a.yzx + 79.76);
    // float n = fract((a.x + a.y) * a.z);
    // return vec2(n, fract((a.x + a.y + n) * a.z));

    vec3 a = fract(vec3(p.xyx) * vec3(213.897, 653.453, 253.098));
    a += dot(a, a.yzx + 79.76);
    float n = fract((a.x + a.y) * a.z);
    
    //periodic variation along the y-axis
    float offsetY = sin(a.y * 0.1 + iTime * 0.5) * 0.0001; // Adjust the frequency and amplitude 
    
    return vec2(n, fract((a.x + a.y + n + offsetY) * a.z));
}




/////////////////////////////////////////////////////
//// Step 1: render a single particle
//// In this function, you are asked to implement the rendering of a single particle onto the screen.
//// The task is to calculate the distance between the current fragment and the particle (both in 2D), 
//// and then use the distance to build a decay function f(d)=1/d, and multiply the function value with brightness and color
//// to calculate the fragment color value.
/////////////////////////////////////////////////////

vec4 renderParticle(vec2 uv, vec2 pos, vec3 color, float num)
{
    vec2 rnd = randMov(vec2(num,num*2.0));
    vec2 point = vec2(cos(iTime*.5*rnd.x+num)*2.0,sin(iTime*.5*rnd.y+num));
    float pointDistance = distance(uv, point)*2.0;
    // Adjust the size of the point light contribution based on distance
    vec3 firefly = vec3(.002 / pointDistance) * vec3(sin(iTime + num) / 12.0);
    return vec4(firefly/pointDistance* color, firefly / pointDistance);
}



void mainImage(out vec4 outputColor, in vec2 fragCoord)
{
    // //// fragPos's center is at the center of the screen, fragPos.y range is [-0.5, 0.5]
    vec2 fragPos = (fragCoord - .5 * iResolution.xy) / iResolution.y;
    
    vec2 uv = fragCoord / (iResolution.xy / 2.0) - 1.0;
    uv.x *= iResolution.x / iResolution.y;


    vec2 pos = vec2((hash1d(2.) * 2. - 1.) * 0.7, -0.5);
    vec3 color = vec3(0.9451, 0.9059, 0.3569);

    vec4 firefly;
    for (float i = 0.0; i < NUM_FIREFLIES; i += 1.0)
    {
        firefly += renderParticle(uv, pos, color, i);

    }
    // Adjust overall brightness of the point light
    

    outputColor = firefly;
}

void main()
{
    mainImage(outputColor, fragCoord);
}