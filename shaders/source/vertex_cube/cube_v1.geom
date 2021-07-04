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
layout(triangle_strip, max_vertices=14) out;

layout(location = 0) in vec3 color[];

layout(location = 0) out vec3 fragColor;


// Vertices
const float a = 0.1f;
const vec3 frontTopLeft =     vec3(-a,  a,  a);    
const vec3 frontTopRight =    vec3( a,  a,  a);   
const vec3 frontBottomLeft =  vec3(-a, -a,  a); 
const vec3 frontBottomRight = vec3( a, -a,  a);     

const vec3 BackTopLeft =      vec3(-a,  a, -a);  
const vec3 BackTopRight =     vec3( a,  a, -a);    
const vec3 BackBottomLeft =   vec3(-a, -a, -a);   
const vec3 BackBottomRight =  vec3( a, -a, -a);   

const int cubeSize = 14;
const vec3 cube[cubeSize] = 
{
    frontTopLeft, 
    frontTopRight, 
    frontBottomLeft, 
    frontBottomRight, 

    BackBottomRight, 

    frontTopRight, 
    BackTopRight, 

    frontTopLeft, 
    BackTopLeft, 

    frontBottomLeft,
    BackBottomLeft, 

    BackBottomRight, 

    BackTopLeft, 
    BackTopRight
};

// Normals
const vec3 front =  vec3( 0.0f,  0.0f,  1.0f);    
const vec3 back =   vec3( 0.0f,  0.0f, -1.0f);    
const vec3 left =   vec3(-1.0f,  0.0f,  0.0f);    
const vec3 right =  vec3( 1.0f,  0.0f,  0.0f);    
const vec3 top =    vec3( 0.0f,  1.0f,  0.0f);    
const vec3 bottom = vec3( 0.0f, -1.0f,  0.0f);    

// Lighting
const vec3 lightPos = vec3(1.0f, 0.5f, 0.7f);
const vec3 lightDir = normalize(lightPos);
const float ambient = 0.2;
const float diffuse = 0.5;

const float strenghtFront =  ambient + max(0,dot(lightDir, front));
const float strenghtBack =   ambient + max(0,dot(lightDir, back));
const float strenghtLeft =   ambient + max(0,dot(lightDir, left));
const float strenghtRight =  ambient + max(0,dot(lightDir, right));
const float strenghTop =     ambient + max(0,dot(lightDir, top));
const float strenghtBottom = ambient + max(0,dot(lightDir, bottom));

const float strenght[cubeSize] =
{
    strenghtFront,
    strenghtFront,
    strenghtFront,
    strenghtFront,

    strenghtBottom,

    strenghtRight,
    strenghtRight,

    strenghTop,
    strenghTop,

    strenghtLeft,
    strenghtLeft,

    strenghtBottom,

    strenghtBack,
    strenghtBack
};



void main()
{	
    const mat4 mvp = ubo.proj * ubo.view * ubo.model;

    for(int i = 0; i < 6; ++i)
    {
        fragColor = color[0] * strenght[i];
        const vec4 pos = gl_in[0].gl_Position + vec4(cube[i], 0.0f);

        gl_Position = mvp * pos;
        EmitVertex();
    }

    //EndPrimitive();
}  