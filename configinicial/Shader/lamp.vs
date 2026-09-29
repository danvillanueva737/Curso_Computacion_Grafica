#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexCoord;

out vec3 Color;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Permiten seleccionar una región de la textura
uniform vec2 texOffset;
uniform vec2 texScale;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);

    Color = inColor;

    // Convierte las coordenadas 0-1 del plano
    // en coordenadas correspondientes a una cara del dado
    TexCoord = inTexCoord * texScale + texOffset;
}