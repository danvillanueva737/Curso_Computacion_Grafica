#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float intensity;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;

uniform Material material;

uniform Light sunLight;
uniform Light moonLight;

uniform sampler2D texture_diffuse1;


// ======================================
// CÁLCULO DE UNA FUENTE DE LUZ
// ======================================

vec3 CalculateLight(
    Light light,
    vec3 norm,
    vec3 viewDir
)
{
    // Ambiental
    vec3 ambient =
        light.ambient *
        material.ambient;

    // Dirección de la luz
    vec3 lightDir =
        normalize(
            light.position - FragPos
        );

    // Difusa
    float diff =
        max(
            dot(norm, lightDir),
            0.0
        );

    vec3 diffuse =
        light.diffuse *
        diff *
        material.diffuse;

    // Especular
    vec3 reflectDir =
        reflect(
            -lightDir,
            norm
        );

    float spec =
        pow(
            max(
                dot(viewDir, reflectDir),
                0.0
            ),
            material.shininess
        );

    vec3 specular =
        light.specular *
        spec *
        material.specular;

    // Intensidad según altura de la fuente
    return (
        ambient +
        diffuse +
        specular
    ) * light.intensity;
}


void main()
{
    vec3 norm =
        normalize(Normal);

    vec3 viewDir =
        normalize(
            viewPos - FragPos
        );

    // Aporte del Sol
    vec3 sunResult =
        CalculateLight(
            sunLight,
            norm,
            viewDir
        );

    // Aporte de la Luna
    vec3 moonResult =
        CalculateLight(
            moonLight,
            norm,
            viewDir
        );

    vec3 result =
        sunResult +
        moonResult;

    vec4 textureColor =
        texture(
            texture_diffuse1,
            TexCoords
        );

    color =
        vec4(result, 1.0f) *
        textureColor;
}