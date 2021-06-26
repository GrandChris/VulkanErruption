#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 lineColor;
layout(location = 1) in vec3 baseColor;
layout(location = 2) in vec3 barycentricCoordinates;

layout(location = 0) out vec4 outColor;

void main() 
{
    const float strength = 1.0f / (1.0f + 131072.0f * barycentricCoordinates.x * barycentricCoordinates.x * barycentricCoordinates.x);

    outColor = max(vec4(lineColor, 1.0f) * strength, vec4(baseColor, 1.0f));
}  