#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 color;
    uvec2 maxIndex;
} ubo;

layout(points) in;

// layout(points, max_vertices=12) out;
// layout(line_strip, max_vertices=12) out;
layout(triangle_strip, max_vertices=12) out;

layout(location = 0) in vec3 pos1[];
layout(location = 1) in vec3 pos2[];
layout(location = 2) in vec3 pos3[];
layout(location = 3) in vec3 pos4[];

layout(location = 4) in vec3 inColor[];

layout(location = 0) out vec3 lineColor;
layout(location = 1) out vec3 baseColor;
layout(location = 2) out vec3 barycentricCoordinates;


// Lighting
const vec3 lightPos = vec3(1.0f, 0.5f, 0.7f);
const vec3 lightDir = normalize(lightPos);
const float ambient = 0.2;
const float diffuse = 0.5;

// Base Color
const vec3 base = vec3(0.25f, 0.25f, 0.25f);

void main()
{	
    // ####### Line Color #######
    const vec3 color = inColor[0];

    // ####### Calculate Vectors and Lengths #######
    const vec3 center = (pos1[0] + pos2[0] + pos3[0] + pos4[0]) / 4;

    const vec3 v12 = pos2[0] - pos1[0];
    const vec3 v24 = pos4[0] - pos2[0];
    const vec3 v43 = pos3[0] - pos4[0];
    const vec3 v31 = pos1[0] - pos3[0];

    const vec3 vc1 = pos1[0] - center;
    const vec3 vc2 = pos2[0] - center;
    const vec3 vc3 = pos3[0] - center;
    const vec3 vc4 = pos4[0] - center;

    const float l12 = length(v12);
    const float l24 = length(v24);
    const float l43 = length(v43);
    const float l31 = length(v31);

    const float lc1 = length(vc1);
    const float lc2 = length(vc2);
    const float lc3 = length(vc3);
    const float lc4 = length(vc4);

    // ####### Calculate Barycentric Heights #######

    const vec3 vnorm1 = cross(v12, vc1);
    const vec3 vnorm2 = cross(v24, vc2);
    const vec3 vnorm3 = cross(v43, vc4);
    const vec3 vnorm4 = cross(v31, vc3);

    const float A1 = length(vnorm1);
    const float A2 = length(vnorm2);
    const float A3 = length(vnorm3);
    const float A4 = length(vnorm4);

    const float h1 = A1 / l12 * 2;
    const float h2 = A2 / l24 * 2;
    const float h3 = A3 / l43 * 2;
    const float h4 = A4 / l31 * 2;

    // ####### Calculate Lighting #######

    const vec3 norm1 = normalize(vnorm1); 
    const vec3 norm2 = normalize(vnorm2); 
    const vec3 norm3 = normalize(vnorm3); 
    const vec3 norm4 = normalize(vnorm4); 

    const float strength1 = ambient + max(0,dot(lightDir, norm1));
    const float strength2 = ambient + max(0,dot(lightDir, norm2));
    const float strength3 = ambient + max(0,dot(lightDir, norm3));
    const float strength4 = ambient + max(0,dot(lightDir, norm4));

    const vec3 baseColor1 = base * strength1;
    const vec3 baseColor2 = base * strength2;
    const vec3 baseColor3 = base * strength3;
    const vec3 baseColor4 = base * strength4;

    // ####### Transform Vertices #######

    const mat4 mvp = ubo.proj * ubo.view * ubo.model;

    const vec4 pos1t = mvp * vec4(pos1[0], 1.0f);
    const vec4 pos2t = mvp * vec4(pos2[0], 1.0f);
    const vec4 pos3t = mvp * vec4(pos3[0], 1.0f);
    const vec4 pos4t = mvp * vec4(pos4[0], 1.0f);
    const vec4 centert = mvp * vec4(center, 1.0f);
 
    // ######## Create Triangles #######

    // first triangle
    gl_Position = centert; 
    // barycentricCoordinates = vec3(h1, 0.0f, 0.0f);
    barycentricCoordinates = vec3(1.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor1;
    EmitVertex();

    gl_Position = pos1t; 
    barycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor1;
    EmitVertex();

    gl_Position = pos2t; 
    barycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor1;
    EmitVertex();

    EndPrimitive();

    // second triangle
    gl_Position = centert; 
    // barycentricCoordinates = vec3(h2, 0.0f, 0.0f);
    barycentricCoordinates = vec3(1.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor2;
    EmitVertex();

    gl_Position = pos2t; 
    barycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor2;
    EmitVertex();

    gl_Position = pos4t; 
    barycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor2;
    EmitVertex();

    EndPrimitive();

    // third triangle
    gl_Position = centert; 
    // barycentricCoordinates = vec3(h3, 0.0f, 0.0f);
    barycentricCoordinates = vec3(1.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor3;
    EmitVertex();

    gl_Position = pos4t; 
    barycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor3;
    EmitVertex();

    gl_Position = pos3t; 
    barycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor3;
    EmitVertex();

    EndPrimitive();

    // fourth triangle
    gl_Position = centert; 
    // barycentricCoordinates = vec3(h4, 0.0f, 0.0f);
    barycentricCoordinates = vec3(1.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor4;
    EmitVertex();

    gl_Position = pos3t; 
    barycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor4;
    EmitVertex();

    gl_Position = pos1t; 
    barycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
    lineColor = color;
    baseColor = baseColor4;
    EmitVertex();

    EndPrimitive();
}  