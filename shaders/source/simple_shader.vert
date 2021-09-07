#version 450
// #extension GL_ARB_separate_shader_objects : enable

// layout(binding = 0) uniform UniformBufferObject
// {
//     mat4 model;
//     mat4 view;
//     mat4 proj;
// } ubo;

layout(location = 0) in vec3 inPosition;


void main() {
    //gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    
    gl_Position = vec4(inPosition, 1.0);
    // gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    // gl_PointSize = 1.0;
}