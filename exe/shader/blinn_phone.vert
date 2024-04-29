#version 450
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    //enablePrintf();
    //printf("hello from vertex %d! with world position %^3g...\n", gl_VertexID,(model*vec4(aPos,1.0f)).xyz);
    vs_out.FragPos = (model*vec4(aPos,1.0f)).xyz;
    
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    // vs_out.Normal = normalize(model*vec4(aNormal,0.0f)).xyz;
    // vs_out.FragPos = aPos;
    // vs_out.Normal = aNormal;
    vs_out.TexCoords = aTexCoords;
    gl_Position = projection * view *model* vec4(aPos, 1.0);
}