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

layout(location = 0) in float inHeight1;
layout(location = 1) in float inHeight2;
layout(location = 2) in float inHeight3;
layout(location = 3) in float inHeight4;

layout(location = 4) in float inLightStrength1;
layout(location = 5) in float inLightStrength2;
layout(location = 6) in float inLightStrength3;
layout(location = 7) in float inLightStrength4;

layout(location = 8) in uint inLightColor;

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
    // uint maxY = ubo.maxIndex.y-1;

    uint x =  i % maxX;
    uint y =  i / maxX;

    pos1 = (ubo.model * vec4(x,   y,   inHeight1, 1.0f)).xyz;
    pos2 = (ubo.model * vec4(x+1, y,   inHeight2, 1.0f)).xyz;
    pos3 = (ubo.model * vec4(x  , y+1, inHeight3, 1.0f)).xyz;
    pos4 = (ubo.model * vec4(x+1, y+1, inHeight4, 1.0f)).xyz;

    // gl_Position = ubo.proj * ubo.view * ubo.model * vec4(x1, y1, inHeight[0], 1.0);

    if(ubo.color.x + ubo.color.y + ubo.color.z == 0) {
        fragColor = colorGradient(y);
    }
    else{
        fragColor = ubo.color;
    }

    lightStrength1 = inLightStrength1;
    lightStrength2 = inLightStrength2;
    lightStrength3 = inLightStrength3;
    lightStrength4 = inLightStrength4;

    ligthColor = unpackUnorm4x8(inLightColor);
    
    // gl_PointSize = 1.0f;
}