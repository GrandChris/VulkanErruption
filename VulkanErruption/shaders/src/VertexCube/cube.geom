#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (constant_id = 0) const float CUBE_SIZE = 0.5f;

layout(points) in;

// layout(points, max_vertices=3) out;
// layout(line_strip, max_vertices=3) out;
layout(triangle_strip, max_vertices=24) out;

layout(location = 0) in vec3 color[];

layout(location = 0) out vec3 fragColor;


// Vertices
const float a = CUBE_SIZE;
 vec3 frontTopLeft =     vec3( a, -a,  a);    
 vec3 frontTopRight =    vec3( a,  a,  a);   
 vec3 frontBottomLeft =  vec3( a, -a, -a); 
 vec3 frontBottomRight = vec3( a,  a, -a);     
 vec3 BackTopLeft =      vec3(-a, -a,  a);  
 vec3 BackTopRight =     vec3(-a,  a,  a);    
 vec3 BackBottomLeft =   vec3(-a, -a, -a);   
 vec3 BackBottomRight =  vec3(-a,  a, -a);   
 const int facesSize = 6;
 const int vertPerFace = 4;
 const int cubeSize = facesSize * vertPerFace;
 vec3 cube[cubeSize] = 
{
    frontTopLeft, 
    frontBottomLeft, 
    frontTopRight, 
    frontBottomRight, 

    BackBottomLeft, 
    BackTopLeft, 
    BackBottomRight, 
    BackTopRight, 

    BackTopLeft, 
    BackBottomLeft, 
    frontTopLeft, 
    frontBottomLeft, 

    frontTopRight,
    frontBottomRight,
    BackTopRight,
    BackBottomRight,

    BackTopLeft, 
    frontTopLeft,
    BackTopRight,
    frontTopRight,

    frontBottomLeft, 
    BackBottomLeft,
    frontBottomRight,
    BackBottomRight
};

// Normals
const vec3 front =  vec3( 1.0f,  0.0f,  0.0f);    
const vec3 back =   vec3(-1.0f,  0.0f,  0.0f);    
const vec3 left =   vec3( 0.0f, -1.0f,  0.0f);    
const vec3 right =  vec3( 0.0f,  1.0f,  0.0f);    
const vec3 top =    vec3( 0.0f,  0.0f,  1.0f);    
const vec3 bottom = vec3( 0.0f,  0.0f, -1.0f);  

const vec3 normals[cubeSize] =
{

    front,
    front,
    front,
    front,

    back,
    back,
    back,
    back,

    left,
    left,
    left,
    left,

    right,
    right,
    right,
    right,

    top,
    top,
    top,
    top,

    bottom,
    bottom,
    bottom,
    bottom
};


vec3 ExtractCameraPos_NoScale(const mat4 a_modelView)
{
  mat3 rotMat = mat3(a_modelView);
  vec3 d = vec3(a_modelView[3]);

  vec3 retVec = -d * rotMat;
  return retVec;
}

void main()
{	
    const vec3 viewPos = ExtractCameraPos_NoScale(ubo.view);
    const vec3 viewDir = viewPos - gl_in[0].gl_Position.xyz;


    const mat4 mvp = ubo.proj * ubo.view * ubo.model;

    for(int i = 0; i < facesSize; ++i)
    {
        if(dot(viewDir, normals[i * vertPerFace]) >= 0)
        {
            for(int j = 0; j < vertPerFace; ++j)
            {
                const int index = i * vertPerFace + j;
                fragColor = color[0];
                const vec4 pos = gl_in[0].gl_Position + vec4(cube[index], 0.0f);

                gl_Position = mvp * pos;
                EmitVertex();
            }
            EndPrimitive();
        }
    }
}  