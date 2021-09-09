//
// @file:   advanced_shader_diffuse.vert
// @author: GrandChris
// @date:   2021-09-09
// @brief:  Shader with diffuse lighting
//

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPosition;
    float ambient;
    // float diffuse;
    // float specular;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 position;
layout(location = 1) out vec3 fragColor;


void main() {
    position = (ubo.model * vec4(inPosition, 1.0)).xyz;

    //gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    
    // gl_Position = vec4(inPosition, 1.0);
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;

    // gl_PointSize = 1.0;
}