#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(points) in;

// layout(points, max_vertices=3) out;
// layout(line_strip, max_vertices=3) out;
layout(triangle_strip, max_vertices=24) out;

layout(location = 0) in vec3 color[];

layout(location = 0) out vec3 fragColor;

// specular lighting
layout(location = 1) out vec3 fragPosition;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 lightDirection;
layout(location = 4) out vec3 viewPosition;
layout(location = 5) out vec3 fragBaseColor;


// Vertices
const float a = 0.5f;
const vec3 frontTopLeft =     vec3( a, -a,  a);    
const vec3 frontTopRight =    vec3( a,  a,  a);   
const vec3 frontBottomLeft =  vec3( a, -a, -a); 
const vec3 frontBottomRight = vec3( a,  a, -a);     

const vec3 BackTopLeft =      vec3(-a, -a,  a);  
const vec3 BackTopRight =     vec3(-a,  a,  a);    
const vec3 BackBottomLeft =   vec3(-a, -a, -a);   
const vec3 BackBottomRight =  vec3(-a,  a, -a);   

const int facesSize = 6;
const int vertPerFace = 4;
const int cubeSize = facesSize * vertPerFace;
const vec3 cube[cubeSize] = 
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


// Lighting
const vec3 lightPos = vec3(1.0f, 0.7f, 0.5f);
const vec3 lightDir = normalize(lightPos);
const float ambient = 0.2;
const float diffuse = 0.5;

const float strenghtFront =  ambient + diffuse * max(0,dot(lightDir, front));
const float strenghtBack =   ambient + diffuse * max(0,dot(lightDir, back));
const float strenghtLeft =   ambient + diffuse * max(0,dot(lightDir, left));
const float strenghtRight =  ambient + diffuse * max(0,dot(lightDir, right));
const float strenghTop =     ambient + diffuse * max(0,dot(lightDir, top));
const float strenghtBottom = ambient + diffuse * max(0,dot(lightDir, bottom));

const float strenght[cubeSize] =
{

    strenghtFront,
    strenghtFront,
    strenghtFront,
    strenghtFront,

    strenghtBack,
    strenghtBack,
    strenghtBack,
    strenghtBack,

    strenghtLeft,
    strenghtLeft,
    strenghtLeft,
    strenghtLeft,

    strenghtRight,
    strenghtRight,
    strenghtRight,
    strenghtRight,

    strenghTop,
    strenghTop,
    strenghTop,
    strenghTop,

    strenghtBottom,
    strenghtBottom,
    strenghtBottom,
    strenghtBottom
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
    const mat4 mvp = ubo.proj * ubo.view * ubo.model;

    for(int i = 0; i < facesSize; ++i)
    {
        for(int j = 0; j < vertPerFace; ++j)
        {
            const int index = i * vertPerFace + j;
            fragColor = color[0] * strenght[index];
            const vec4 pos = gl_in[0].gl_Position + vec4(cube[index], 0.0f);

            gl_Position = mvp * pos;
            
            // for specular lighting
            fragPosition = vec3(ubo.model * pos);
            fragBaseColor = color[0];
            fragNormal = normals[index], 1.0f;
            lightDirection = lightDir, 1.0f;
            viewPosition = ExtractCameraPos_NoScale(ubo.view);

            EmitVertex();
        }
        EndPrimitive();
    }
}  