#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
// layout(location = 2) in float inPointSize;

layout(location = 0) out vec3 fragColor;

#define M_PI 3.1415926535897932384626433832795

void main() 
{
    gl_Position = vec4(inPosition, 1.0);
    
    // gl_PointSize = inPointSize;
    fragColor = inColor;
}