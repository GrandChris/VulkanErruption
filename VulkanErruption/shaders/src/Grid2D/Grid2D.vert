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

layout(location = 0) in float inHeight1;
layout(location = 1) in float inHeight2;
layout(location = 2) in float inHeight3;
layout(location = 3) in float inHeight4;

layout(location = 0) out vec3 pos1;
layout(location = 1) out vec3 pos2;
layout(location = 2) out vec3 pos3;
layout(location = 3) out vec3 pos4;

layout(location = 4) out vec3 fragColor;


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
    uint maxX = ubo.maxIndex.x-1;
    // uint maxY = ubo.maxIndex.y-1;

    uint x =  i % maxX;
    uint y =  i / maxX;

    pos1 = vec3(x,   y,   inHeight1);
    pos2 = vec3(x+1, y,   inHeight2);
    pos3 = vec3(x  , y+1, inHeight3);
    pos4 = vec3(x+1, y+1, inHeight4);

    // gl_Position = ubo.proj * ubo.view * ubo.model * vec4(x1, y1, inHeight[0], 1.0);
    // fragColor = colorGradient(i*i);
    fragColor = ubo.color;

    // gl_PointSize = 1.0f;
}