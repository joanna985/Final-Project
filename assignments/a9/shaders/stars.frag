#version 330 core

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;

uniform sampler2D tex_buzz; 

in vec3 vtx_pos; // [-1, 1]
in vec2 vtx_uv; // [0, 1]

out vec4 frag_color;

#define NUM_STAR 100.

// return random vec2 between 0 and 1
vec2 hash2d(float t) {
    t += 1.;
    float x = fract(sin(t * 674.3) * 453.2);
    float y = fract(sin((t + x) * 714.3) * 263.2);
    return vec2(x, y);
}

float rand(float n) {return fract(sin(n) * 43758.5453123);}


vec3 renderParticle(vec2 uv, vec2 pos, float brightness, vec3 color) {
    float d = length(uv - pos);
    return brightness / d * color;
}


vec3 renderStars(vec2 uv) {
    vec3 fragColor = vec3(0.0);
    float t = iTime;
    for(float i = 0.; i < NUM_STAR; i++)
    {
        vec2 pos = hash2d(i) * 2. - 1.; // map to [-1, 1]
        float brightness = .0015;
        brightness *= sin(1.5 * t + i) * .5 + .5; // flicker
        vec3 color = vec3(0.15, 0.71, 0.92);

        fragColor += renderParticle(uv, pos, brightness, color);
    }

    return fragColor;
}

// vec3 renderFireflies(vec2 uv) {
//     vec3 fragColor = vec3(0.0);
//     vec2 mouse = iMouse.xy / iResolution.xy;
//     float time = iTime;
//     float ratio = iResolution.y / iResolution.x;
//     vec2 uv2 = (clamp(abs(uv), 0.8, 1.0) - 0.8) * 5.0; 
//     float r = 0.0005 * mix(1.0, 0.0, max(uv2.x, uv2.y));
//     uv.y *= ratio;
//     float l = .5;
//     float brightness = .0015;
//     for (float i = 0.0; i < 40.0; i += 1.0) {
//         float r1 = rand(i);
//         float r2 = rand(i * 1.3);
//         vec2 p = mod(vec2(
//             13.0 * r1 + 0.1 * cos(r1 * time + r1) - (mouse.x / floor(5.0 + i / 10.0) * 2.0),
//             13.0 * r2 + 0.1 * sin(r2 * time + r2) - (mouse.y / floor(5.0 + i / 10.0) * 2.0)
//         ), 2.0) - 1.0;
//         float d = distance(uv, p);
//         l += pow(r / d * (sin(time + i) + 1.0), 1.5);

        
//         brightness *= sin(1.5 * time + i) * .5 + .5; // flicker
//     }
    
//     fragColor += vec3(1.0, 0.6, 0.0) * brightness; // Use l to modulate brightness

//     return fragColor;
// }




void main() {
    // Calculate horizontal position (range from 0.0 to 1.0)
    float horizontalPosition = (vtx_pos.y + 1.0) / 2.0;

    // Define colors for the gradient
    vec3 black = vec3(0.0, 0.0, 0.0) * 20.5;    // Orange
    vec3 navy = vec3(0.0, 0.0667, 0.1765) * 5.5; // Light Yellow
    vec3 lightBlue = vec3(0.0275, 0.1137, 0.2) * 5.5;  // Light Blue
    vec3 navy2 = vec3(0.0353, 0.0, 0.2392) * 15.5;       // Navy
    vec3 darkNavy = vec3(0.0, 0.0, 0.0) * 15.5;   // Dark Navy (new color)

    // Calculate color transitions
    vec3 blackToNavy = mix(black, navy, horizontalPosition);
    vec3 navyToBlue = mix(navy, lightBlue, horizontalPosition);
    vec3 blueToNavy = mix(lightBlue, navy2, horizontalPosition);
    vec3 navyToDarkNavy = mix(navy2, darkNavy, horizontalPosition); // New transition

    // Interpolate between the final colors based on horizontal position
    vec3 background_color = mix(blackToNavy, navyToBlue, horizontalPosition);
    background_color = mix(background_color, blueToNavy, horizontalPosition);
    background_color = mix(background_color, navyToDarkNavy, horizontalPosition); // Add new color transition

    // Render starry effect
    vec3 starColor = renderStars(vtx_pos.xy);

    // renderFireflies

    //vec3 fireflies = renderFireflies(vtx_pos.xy);



    // Combine starry effect with background color
    frag_color = vec4(mix(background_color, starColor, 0.8), 1.0); // Fully opaque

}



