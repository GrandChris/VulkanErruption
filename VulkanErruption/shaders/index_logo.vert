#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
    float time;
} ubo;

layout(location = 0) in vec3 inStartPosition;
layout(location = 1) in vec3 inStopPosition;
layout(location = 2) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

#define M_PI 3.1415926535897932384626433832795

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inStartPosition + (inStopPosition - inStartPosition) * cos( 2 * M_PI * abs(ubo.time)/4) , 1.0);
    
    gl_PointSize = 1.0f;
    fragColor = inColor;
}