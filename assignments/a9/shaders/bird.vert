#version 330 core

layout(location = 0) in vec3 inPosition;

out vec2 fragCoord;

void main()
{
    gl_Position = vec4(inPosition, 1.0);
    fragCoord = inPosition.xy * 0.5 + 0.5; // Pass through texture coordinates
}