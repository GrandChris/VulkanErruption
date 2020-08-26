#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPos; 
    vec3 color; 
    uvec2 maxIndex;
} ubo;

layout (constant_id = 0) const int USE_SPECULAR = 1;

layout(points) in;

// layout(points, max_vertices=12) out;
// layout(line_strip, max_vertices=12) out;
layout(triangle_strip, max_vertices=12) out;

layout(location = 0) in vec3 pos1[];
layout(location = 1) in vec3 pos2[];
layout(location = 2) in vec3 pos3[];
layout(location = 3) in vec3 pos4[];

layout(location = 4) in vec3 inColor[];

layout(location = 5) in float lightStrength1[];
layout(location = 6) in float lightStrength2[];
layout(location = 7) in float lightStrength3[];
layout(location = 8) in float lightStrength4[];

layout(location = 9) in vec4 customLightColor[];

layout(location = 0) out vec3 outLineColor;
layout(location = 1) out vec3 outBaseColor;
layout(location = 2) out vec3 outBarycentricCoordinates;

// Base Color
const vec3 lightColor = vec3(1.0f, 1.0f, 0.7f);
const vec3 baseColor  = vec3(0.0f, 0.0f, 0.0f);

// Specular Lighting
vec3 ExtractCameraPos_NoScale(const mat4 a_modelView)
{
  mat3 rotMat = mat3(a_modelView);
  vec3 d = vec3(a_modelView[3]);

  vec3 retVec = -d * rotMat;
  return retVec;
}

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
    const float ambientStrength = 0.01;
    const float diffuseStrength = 0.2;

    // const vec3 ligthPos = vec3(0.0f, 10.0f, 10.0f);
    const vec3 lightDir = normalize(ubo.lightPos - center);

    const vec3 norm1 = normalize(vnorm1); 
    const vec3 norm2 = normalize(vnorm2); 
    const vec3 norm3 = normalize(vnorm3); 
    const vec3 norm4 = normalize(vnorm4); 

    const vec3 diffuse1 = lightColor * diffuseStrength * max(0,dot(-lightDir, norm1));
    const vec3 diffuse2 = lightColor * diffuseStrength * max(0,dot(-lightDir, norm2));
    const vec3 diffuse3 = lightColor * diffuseStrength * max(0,dot(-lightDir, norm3));
    const vec3 diffuse4 = lightColor * diffuseStrength * max(0,dot(-lightDir, norm4));

    // ####### Specular Calculate Lighting #######
    const float specularStrength = 0.1;

    const vec3 viewPos = ExtractCameraPos_NoScale(ubo.view);

    const vec3 viewDir = normalize(viewPos - center);

    vec3 reflectDir1 = reflect(lightDir, norm1);  
    vec3 reflectDir2 = reflect(lightDir, norm2);  
    vec3 reflectDir3 = reflect(lightDir, norm3);  
    vec3 reflectDir4 = reflect(lightDir, norm4);  

    float spec1 = pow(max(dot(-viewDir, reflectDir1), 0.0), 128);
    float spec2 = pow(max(dot(-viewDir, reflectDir2), 0.0), 128);
    float spec3 = pow(max(dot(-viewDir, reflectDir3), 0.0), 128);
    float spec4 = pow(max(dot(-viewDir, reflectDir4), 0.0), 128);

    const vec3 specular1 = lightColor * specularStrength * spec1;
    const vec3 specular2 = lightColor * specularStrength * spec2;
    const vec3 specular3 = lightColor * specularStrength * spec3;
    const vec3 specular4 = lightColor * specularStrength * spec4;

    //  ####### Outside Custom Lighting #######

    const float lightStrengthCenter = (lightStrength1[0] + lightStrength2[0] + lightStrength3[0] + lightStrength4[0]) / 4.0f;

    const vec3 customLightCenter = customLightColor[0].xyz * lightStrengthCenter;
    const vec3 customLight1 = customLightColor[0].xyz * lightStrength1[0];
    const vec3 customLight2 = customLightColor[0].xyz * lightStrength2[0];
    const vec3 customLight3 = customLightColor[0].xyz * lightStrength3[0];
    const vec3 customLight4 = customLightColor[0].xyz * lightStrength4[0];

    //  ####### Combine Lightings #######

    vec3 base1 =  baseColor + ambientStrength + diffuse1;
    vec3 base2 =  baseColor + ambientStrength + diffuse2;
    vec3 base3 =  baseColor + ambientStrength + diffuse3;
    vec3 base4 =  baseColor + ambientStrength + diffuse4;

    if(USE_SPECULAR == 1) {
        base1 = base1 + specular1;
        base2 = base2 + specular2;
        base3 = base3 + specular3;
        base4 = base4 + specular4;
    }

    // ####### Transform Vertices #######

    const mat4 vp = ubo.proj * ubo.view;

    const vec4 pos1t = vp * vec4(pos1[0], 1.0f);
    const vec4 pos2t = vp * vec4(pos2[0], 1.0f);
    const vec4 pos3t = vp * vec4(pos3[0], 1.0f);
    const vec4 pos4t = vp * vec4(pos4[0], 1.0f);
    const vec4 centert = vp * vec4(center, 1.0f);
 
    // ######## Create Triangles #######

    // first triangle
    if(dot(viewDir, norm1) < 0)
    {
        gl_Position = centert; 
        // barycentricCoordinates = vec3(h1, 0.0f, 0.0f);
        outBarycentricCoordinates = vec3(1.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base1 + customLightCenter;
        EmitVertex();

        gl_Position = pos1t; 
        outBarycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base1 + customLight1;
        EmitVertex();

        gl_Position = pos2t; 
        outBarycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base1 + customLight2;
        EmitVertex();

        EndPrimitive();
    }



    // second triangle
    if(dot(viewDir, norm2) < 0)
    {
        gl_Position = centert; 
        // barycentricCoordinates = vec3(h2, 0.0f, 0.0f);
        outBarycentricCoordinates = vec3(1.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base2 + customLightCenter;
        EmitVertex();

        gl_Position = pos2t; 
        outBarycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base2 + customLight2;
        EmitVertex();

        gl_Position = pos4t; 
        outBarycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base2 + customLight4;
        EmitVertex();

        EndPrimitive();
    }

    // third triangle
    if(dot(viewDir, norm3) < 0)
    {
        gl_Position = centert; 
        // barycentricCoordinates = vec3(h3, 0.0f, 0.0f);
        outBarycentricCoordinates = vec3(1.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base3 + customLightCenter;
        EmitVertex();

        gl_Position = pos4t; 
        outBarycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base3 + customLight4;
        EmitVertex();

        gl_Position = pos3t; 
        outBarycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base3 + customLight3;
        EmitVertex();

        EndPrimitive();
    }

    // fourth triangle
    if(dot(viewDir, norm4) < 0)
    {
        gl_Position = centert; 
        // barycentricCoordinates = vec3(h4, 0.0f, 0.0f);
        outBarycentricCoordinates = vec3(1.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base4 + customLightCenter;
        EmitVertex();

        gl_Position = pos3t; 
        outBarycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base4 + customLight3;
        EmitVertex();

        gl_Position = pos1t; 
        outBarycentricCoordinates = vec3(0.0f, 0.0f, 0.0f);
        outLineColor = color;
        outBaseColor = base4 + customLight1;
        EmitVertex();

        EndPrimitive();
    }
}  