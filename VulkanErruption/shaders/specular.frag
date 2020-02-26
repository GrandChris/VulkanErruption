#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

// specular lighting
layout(location = 1) in vec3 fragPosition;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 lightDirection;
layout(location = 4) in vec3 viewPosition;
layout(location = 5) in vec3 fragBaseColor;

layout(location = 0) out vec4 outColor;

const float specularStrength = 0.5;



void main() 
{   
    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(lightDirection, fragNormal);  
    float spec = pow(max(dot(-viewDir, reflectDir), 0.0), 32);

    vec3 specular = specularStrength * spec * fragBaseColor;

    outColor = vec4(fragColor + specular, 1.0);
}