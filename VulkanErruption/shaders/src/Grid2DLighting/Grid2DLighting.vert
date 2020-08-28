#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(std140, binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightPos; 
    vec3 color;
    uvec2 maxIndex;
} ubo;

struct StorageBufferElement
{
    float height;
    float lightStrength;
};

layout(std430, binding = 1) readonly buffer StorageBufferObject
{
    StorageBufferElement elems[];
} sbo;

layout(location = 0) in uint inLightColor;

layout(location = 0) out vec3 pos1;
layout(location = 1) out vec3 pos2;
layout(location = 2) out vec3 pos3;
layout(location = 3) out vec3 pos4;

layout(location = 4) out vec3 fragColor;

layout(location = 5) out float lightStrength1;
layout(location = 6) out float lightStrength2;
layout(location = 7) out float lightStrength3;
layout(location = 8) out float lightStrength4;

layout(location = 9) out vec4 ligthColor;


const float divider = 1.0f / 255.0f;

const int colorSize = 7;
const int valsPerColor = 255 / (colorSize-1);
const float valsPerColorInv = 1.0f / valsPerColor;
const vec3 colors[colorSize] = {
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f }
	};

vec3 colorGradient(uint val1)
{
    uint val = val1 % 255;

    int index = int(val * valsPerColorInv);

    float ratio = valsPerColorInv * (val -  valsPerColor * index);
    float ratioInv = 1.0f - ratio;

	return ratioInv * colors[index] + ratio * colors[index + 1];
}

vec3 colorGradient2(uint val)
{
    const float varR = val * 0.02f;
    const float varG = val * 0.03f;
    const float varB = val * 0.05f;

    const float r = (sin(varR) + 1.0f) * 0.5f;
    const float g = (sin(varG) + 1.0f) * 0.5f;
    const float b = (sin(varB) + 1.0f) * 0.5f;

    return vec3(r, g, b);
}


void main() 
{
    uint i = gl_VertexIndex;
    uint maxX = ubo.maxIndex.x;
    // uint maxY = ubo.maxIndex.y;

    uint x =  i % (maxX-1);
    uint y =  i / (maxX-1);

    // for every point, it also needs the height of the 3 neighbouring points
    //  1------2
    //  |      |
    //  |      |
    //  3------4

    const uint x1 = x;
    const uint x2 = x+1;
    const uint y1 = y;
    const uint y2 = y+1;

    const uint index1 = y1 * maxX + x1;
    const uint index2 = y1 * maxX + x2;
    const uint index3 = y2 * maxX + x1;
    const uint index4 = y2 * maxX + x2;

    const float h1 = sbo.elems[index1].height;
    const float h2 = sbo.elems[index2].height;
    const float h3 = sbo.elems[index3].height;
    const float h4 = sbo.elems[index4].height;

    const float light1 = sbo.elems[index1].lightStrength;
    const float light2 = sbo.elems[index2].lightStrength;
    const float light3 = sbo.elems[index3].lightStrength;
    const float light4 = sbo.elems[index4].lightStrength;

    pos1 = (ubo.model * vec4(x1,   y1,  h1, 1.0f)).xyz;
    pos2 = (ubo.model * vec4(x2,   y1,  h2, 1.0f)).xyz;
    pos3 = (ubo.model * vec4(x1,   y2,  h3, 1.0f)).xyz;
    pos4 = (ubo.model * vec4(x2,   y2,  h4, 1.0f)).xyz;

    // gl_Position = ubo.proj * ubo.view * ubo.model * vec4(x1, y1, inHeight[0], 1.0);

    if(ubo.color.x + ubo.color.y + ubo.color.z == 0) {
        fragColor = colorGradient(y);
    }
    else{
        fragColor = ubo.color;
    }

    lightStrength1 = light1;
    lightStrength2 = light2;
    lightStrength3 = light3;
    lightStrength4 = light4;

    ligthColor = unpackUnorm4x8(inLightColor);
    
    // gl_PointSize = 1.0f;
}