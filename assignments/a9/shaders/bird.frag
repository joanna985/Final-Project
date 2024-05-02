#version 330 core

uniform vec2 iResolution;
uniform float iTime; // shader playback time (in seconds)

out vec4 frag_color;

#define PI 3.14159265358979323846

// Define the height of the floor in the scene
#define FLOOR_Y 4.

// Function to generate pseudo-random noise
float hash(vec3 p)  
{
    // Hash function to generate pseudo-random values
    p  = fract( p*0.3183099+.1 );
	p *= 17.0;
    return fract( p.x*p.y*p.z*(p.x+p.y+p.z) );
}

// Function to generate Perlin noise
float noise( in vec3 x )
{
    // Get the integer part of the input coordinates
    vec3 i = floor(x);
    // Get the fractional part of the input coordinates
    vec3 f = fract(x);
    // Smoothstep function to smooth the noise
    f = f*f*(3.0-2.0*f);
	
    // Interpolate between the hash values at the corners of the unit cube
    return mix(mix(mix( hash(i+vec3(0,0,0)), 
                        hash(i+vec3(1,0,0)),f.x),
                   mix( hash(i+vec3(0,1,0)), 
                        hash(i+vec3(1,1,0)),f.x),f.y),
               mix(mix( hash(i+vec3(0,0,1)), 
                        hash(i+vec3(1,0,1)),f.x),
                   mix( hash(i+vec3(0,1,1)), 
                        hash(i+vec3(1,1,1)),f.x),f.y),f.z);
}

// Transformation matrix for bending
const mat3 m = mat3( 0.00,  0.80,  0.60,
                    -0.80,  0.36, -0.48,
                    -0.60, -0.48,  0.64 );

// Function to generate fractal noise
float fnoise( in vec3 pos )
{
    float f = 0.0;
	vec3 q = 8.0*pos;
    f  = 0.5000*noise( q ); q = m*q*2.01;
    f += 0.2500*noise( q ); q = m*q*2.02;
    f += 0.1250*noise( q ); q = m*q*2.03;
    f += 0.0625*noise( q ); q = m*q*2.01;
    return f;
}

// Function to calculate the distance to a sphere
float sdSphere( vec3 p, float s )
{
  return length(p)-s;
}

// Function to calculate the distance to a box
float sdBox( vec3 p, vec3 b )
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

// Function to calculate the distance to a capsule
float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
  vec3 pa = p - a, ba = b - a;
  float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return length( pa - ba*h ) - r;
}

// Function to calculate the distance to a rounded cone
float sdRoundCone( vec3 p, float r1, float r2, float h )
{
  vec2 q = vec2( length(p.xz), p.y );
    
  float b = (r1-r2)/h;
  float a = sqrt(1.0-b*b);
  float k = dot(q,vec2(-b,a));
    
  if( k < 0.0 ) return length(q) - r1;
  if( k > a*h ) return length(q-vec2(0.0,h)) - r2;
        
  return dot(q, vec2(a,b) ) - r1;
}

// Function to calculate the distance to a plane
float sdPlane( vec3 p)
{
	return abs(p.y);
}

// Function to smooth union of two shapes
float opSmoothUnion( float d1, float d2, float k ) 
{
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h); 
}

// Function to bend an object
vec3 opCheapBend(in vec3 p , float amount)
{
    float k = amount; // or some other amount
    float c = cos(k*p.x);
    float s = sin(k*p.x);
    mat2  m = mat2(c,-s,s,c);
    vec3  q = vec3(m*p.xy,p.z);
    return q;
}

// Function to create a rotation matrix
mat4 rotationMatrix(vec3 axis, float angle) {
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

// Function to rotate a point around an axis
vec3 opRotate(vec3 v, vec3 axis, float angle) {
	mat4 m = rotationMatrix(axis, angle);
	return (m * vec4(v, 1.0)).xyz;
}

// Function to model the bird
float model(vec3 p)
{
    // set up
    p*=.9;
    p.y -= .5;
    
    float bird;
    
    // movement
    p.y += -1. - sin(iTime*14.) * .2 * sin(iTime + 0.5);
    
    // body
    float body;
    {
    	vec3 bp = p;
        bp *= 1.;
        bp.z += 1.25;
        bp.y *= 1.25;
        bp.y *= .6 + sin(bp.z*4. + 3.)*.1;
        bp.z *= 1.2;
        
        body = sdCapsule( bp, vec3(0., -.25, .0), vec3(.15, .1, 2.), .3);
    }
    bird = body;
    
    // head
    float head;
    {
        vec3 hp = p;
        hp.z -= .7;
        hp.x -= .1;
        hp.y -= .2;
        
        head = sdSphere(hp, .24);
    }
    bird = opSmoothUnion(bird, head, .2);
    
    // beak
    float beak;
    {
        vec3 bp = p;
        bp.z -= 1.2;
        bp.x -= .1;
        bp.y -= .1;
        bp.y += bp.z*.25;
        bp = opRotate(bp, vec3(1., 0., 0.), - PI * .6);
        
        beak = sdRoundCone( bp, .025, .1, .3 ) * .8;
    }
    bird = opSmoothUnion(bird, beak, .1);
    
    // wings
    float wings = 9999999.;
    {
    	vec3 wp = opCheapBend(p, sin(iTime*14.) * .2 * sin(iTime));
        wp.y -= .15;
        
        if(wp.z < .35)
        		wp.z += sin(wp.x * 30.) * .05 * wp.z;
        
       	if(wp.z < -.1)
            wp.z += sin(abs(wp.x) * 1.6) * .3;
        else
            wp.z += sin(abs(wp.x) * 2.75) * .1;
        
        wp.y *= 1. - sin(wp.z*25.)*.05;
        
        wings = sdBox(wp, vec3(2.8, .05, .4));
        
        wings *= .4;
    }
    bird = opSmoothUnion(bird, wings, .05); 
    
    // talons
    float talons = 999999.;
    {
        for(float i = -1.; i <= 1.; i+=2.)
        {
            float index = 0.;
            for(float j = 3.; j > 2.; j-=.3)
            {
        		vec3 wp = p;
                
        		wp = opCheapBend(wp, sin(iTime*14.) * .2 * sin(iTime));
                
        		wp.x += j * i;
            	wp.x += .1;
        		wp.z += .6;
        		wp.y -= .2;
        		wp.y *= 1.1;
                wp.y += (index/3.)*.2;
              
        
        		talons = min(talons, sdCapsule( wp, vec3(0., 0., 0.), vec3(.1, .1, .7), .025));
                
                index++;
                
            }
        }
    }
    bird = opSmoothUnion(bird, talons, .3); 
    
    // tail
    float tail;
    {
    	vec3 wp = p;
        wp *= .7;
        
        wp.y += .1;
        wp.z -= .7;
        wp.z *= 1.2;
        wp.x *= .6;
        
        wp.z += sin(wp.x + PI/2.)*2.;
        wp.x *= 1.5;
        
        wp.x *= 1. + wp.z*.9;
        wp.x *= 1.4;
        
        wp.y *= 1. - sin(wp.x*25.)*.3;
        
    	tail = sdBox(wp, vec3(.5, .05, .4)) * .7;
    }
    bird = opSmoothUnion(bird, tail, .25);
    
    return bird;
}

// Function to perform ray marching
float raymarch(in vec3 ro, in vec3 rd)
{
    float dist = 0.;
    for(int i = 0; i < 90; i++)
    {
		float m = model(ro+rd*dist);
        dist += m;
        
        if(m < .05) return dist;
        else if(dist > 30.) break;
    }
    return -1.;
}

// Function to calculate the surface normal at a given point
vec3 normal(vec3 pos)
{
    vec3 eps = vec3(.01, -.01, 0.);
    
    return normalize(vec3(
        model(pos + eps.xzz) - model(pos + eps.yzz),
        model(pos + eps.zxz) - model(pos + eps.zyz),
        model(pos + eps.zzx) - model(pos + eps.zzy)));
}

// Function to calculate the shadow
float shadow(in vec3 pos, in vec3 ld)
{
    float spread = 3.;
    float res = 1.0;
	for(float t = .2; t < .4;)
    {
        float dist = model(pos+ld*t);
        if(dist<.001) return 0.;
        res = min(res, spread*dist/t);
        t += dist;
    }
    return res;
}

// Function to define the background color
vec3 background()
{
	return vec3(0.);  
}

// Function to shade the scene
vec3 shade(vec3 pos, vec3 nor, vec3 rd, float dist)
{
    if(dist < 0.) return background();
    
    vec3 ld = normalize(vec3(2., 10., 2.));
    
    vec3 lightColor = vec3(.9, .6, .1) * 1.8;
    vec3 birdColor = vec3(0.1686, 0.1294, 0.0078);
    
    float dif = max(dot(nor,ld),0.);
    float sha = 0.;
    if(dif > .01) sha = shadow(pos, ld);
    vec3 lin = vec3(dif*sha);
    
    vec3 col = lin;
    
    // bird
    col *= birdColor;
    
    col = min(col, vec3(1.));
    
    return col;
}

// Main function to render the scene
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 p = (fragCoord - .5*iResolution.xy)/iResolution.y;
    
    float zoom = 3.;
    vec3 ro = vec3(sin(iTime*.5)*zoom, zoom * 2., cos(iTime*.5)*zoom); 
    vec3 ta = vec3(0.);				
    
    vec3 w = normalize (ta-ro);
    vec3 u = normalize (cross (w, vec3(0., 1., 0.)));
    vec3 v = normalize (cross (u, w));
    mat3 mat = mat3(u, v, w);
    vec3 rd = normalize (mat*vec3(p.xy,.5));
    
    float dist = raymarch(ro, rd);
    vec3 pos = ro+rd*dist;
    vec3 nor = normal(pos);
    
    vec3 col = shade(pos, nor, rd, dist);
    
    col = mix(col, background(), pow(dist/30., 2.));
    
    col = vec3(dot(col.rgb, vec3(0.299, 0.587, 0.114)));
    
    // Output to screen
    fragColor = vec4(col,1.0);
}
