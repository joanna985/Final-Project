#version 330 core

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
in vec2 fragCoord;
out vec4 fragColor;

uniform sampler2D bufferTexture;

vec3 gamma2(vec3 col) {
    return vec3(sqrt(col.r), sqrt(col.g), sqrt(col.b));
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 uv = fragCoord.xy / iResolution.xy;
    vec4 col = texture(bufferTexture, uv);
    fragColor = vec4(gamma2(col.rgb), 1.);
}

void main() {
    mainImage(fragColor, fragCoord);
}
