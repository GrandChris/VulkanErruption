#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
    uvec3 maxIndex;
} ubo;

layout(location = 0) in uint inColor;

layout(location = 0) out vec3 fragColor;

#define M_PI 3.1415926535897932384626433832795

const float divider = 1.0f / 255.0f;

const int colorSize = 6;
const int valsPerColor = 255 / (colorSize-1);
const float valsPerColorInv = 1.0f / valsPerColor;
const vec3 colors[colorSize] = {
		{ 1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		//{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f }
	};

vec3 colorGradient(uint val1)
{
    uint val2 = val1 * 300;
    uint val = uint(min(sqrt(val2), 255.0f));


    int index = int(val * valsPerColorInv);

    float ratio = valsPerColorInv * (val -  valsPerColor * index);
    float ratioInv = 1.0f - ratio;

	return ratioInv * colors[index] + ratio * colors[index + 1];
}


void main() 
{
    uint i = gl_VertexIndex;
    uint maxY = ubo.maxIndex.y;
    uint maxX = ubo.maxIndex.x;

    uint x =  i % maxX;
    uint y =  (i / maxX) % maxY;
    uint z =  i / (maxX * maxY);

    if(inColor == 0)
    {
        gl_Position = ubo.proj * ubo.view * ubo.model * vec4(999, 999, 999, 1.0);
    }
    else
    {
        gl_Position = ubo.proj * ubo.view * ubo.model * vec4(x, y, z, 1.0);
    }
    

    fragColor = colorGradient(inColor);

    gl_PointSize = 1.0f;
}