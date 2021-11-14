//
// @file:   cube_shader.frag
// @author: GrandChris
// @date:   2021-09-09
// @brief:  Shader with diffuse lighting
//

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
    // outColor = vec4(0.5f, 0.5f, 0.5f, 1.0);
}