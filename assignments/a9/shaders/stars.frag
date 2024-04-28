#version 330 core

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;

uniform sampler2D tex_buzz; 

in vec3 vtx_pos; // [-1, 1]
in vec2 vtx_uv; // [0, 1]

out vec4 frag_color;

#define NUM_STAR 100.
#define NUM_FIREFLIES 150.

// return random vec2 between 0 and 1
vec2 hash2d(float t) {
    t += 1.;
    float x = fract(sin(t * 674.3) * 453.2);
    float y = fract(sin((t + x) * 714.3) * 263.2);
    return vec2(x, y);
}


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



void main() {
    // Calculate horizontal position (range from 0.0 to 1.0)
    float horizontalPosition = (vtx_pos.y + 1.0) / 2.0;

    // Define colors for the gradient
    vec3 orange = vec3(1.0, 0.8, 0.0) * 20.5;    // Orange
    vec3 lightYellow = vec3(1.0, 1.0, 0.5) * 5.5; // Light Yellow
    vec3 lightBlue = vec3(0.5, 0.5, 1.0) * 5.5;  // Light Blue
    vec3 navy = vec3(0.0, 0.0, 0.2) * 15.5;       // Navy
    vec3 darkNavy = vec3(0.0, 0.0, 0.1) * 15.5;   // Dark Navy (new color)

    // Calculate color transitions
    vec3 orangeToYellow = mix(orange, lightYellow, horizontalPosition);
    vec3 yellowToBlue = mix(lightYellow, lightBlue, horizontalPosition);
    vec3 blueToNavy = mix(lightBlue, navy, horizontalPosition);
    vec3 navyToDarkNavy = mix(navy, darkNavy, horizontalPosition); // New transition

    // Interpolate between the final colors based on horizontal position
    vec3 background_color = mix(orangeToYellow, yellowToBlue, horizontalPosition);
    background_color = mix(background_color, blueToNavy, horizontalPosition);
    background_color = mix(background_color, navyToDarkNavy, horizontalPosition); // Add new color transition

    // Render starry effect
    vec3 starColor = renderStars(vtx_pos.xy);

    // Combine starry effect with background color
    frag_color = vec4(mix(background_color, starColor, 0.8), 1.0); // Fully opaque
}

