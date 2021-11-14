//
// @file:   cube_shader.vert
// @author: GrandChris
// @date:   2021-09-09
// @brief:  Shader with diffuse lighting
//

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in uint inColor;
// layout(location = 1) in vec3 inColor;

layout(location = 0) out uint fragColor;

layout (constant_id = 0) const float CUBE_SIZE = 1.0f;
layout (constant_id = 1) const uint ARRAY_LENGTH = 16;

void main() 
{
    uint i = gl_VertexIndex;
    uint x =  i % ARRAY_LENGTH;
    uint y =  (i / ARRAY_LENGTH) % ARRAY_LENGTH;
    uint z =  i / (ARRAY_LENGTH * ARRAY_LENGTH);

    float xf = x * CUBE_SIZE * 1.5;
    float yf = y * CUBE_SIZE * 1.5;
    float zf = z * CUBE_SIZE * 1.5;

    gl_Position = vec4(xf, yf, zf, 1.0);
    
    // gl_PointSize = inPointSize;
    fragColor = inColor;
}