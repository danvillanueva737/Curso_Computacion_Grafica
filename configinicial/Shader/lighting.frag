
#version 330 core

#define NUMBER_OF_POINT_LIGHTS 4

// Propiedades del material
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// Luz direccional
struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Luz puntual
struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Reflector
struct SpotLight
{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Datos recibidos del vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Salida del fragment shader
out vec4 color;

// Uniforms
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

uniform float objectAlpha;
uniform int transparency;


// ==========================================
// FUNCION DE ILUMINACION DIRECCIONAL
// ==========================================
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(
        max(dot(viewDir, reflectDir), 0.0),
        material.shininess
    );

    vec3 ambient = light.ambient *
        texture(material.diffuse, TexCoords).rgb;

    vec3 diffuse = light.diffuse * diff *
        texture(material.diffuse, TexCoords).rgb;

    vec3 specular = light.specular * spec *
        texture(material.specular, TexCoords).rgb;

    return ambient + diffuse + specular;
}


// ==========================================
// FUNCION DE ILUMINACION PUNTUAL
// ==========================================
vec3 CalcPointLight(
    PointLight light,
    vec3 normal,
    vec3 fragPos,
    vec3 viewDir
)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(
        max(dot(viewDir, reflectDir), 0.0),
        material.shininess
    );

    float distance = length(light.position - fragPos);

    float attenuation = 1.0 /
        (light.constant +
         light.linear * distance +
         light.quadratic * distance * distance);

    vec3 ambient = light.ambient *
        texture(material.diffuse, TexCoords).rgb;

    vec3 diffuse = light.diffuse * diff *
        texture(material.diffuse, TexCoords).rgb;

    vec3 specular = light.specular * spec *
        texture(material.specular, TexCoords).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}


// ==========================================
// FUNCION DE ILUMINACION SPOTLIGHT
// ==========================================
vec3 CalcSpotLight(
    SpotLight light,
    vec3 normal,
    vec3 fragPos,
    vec3 viewDir
)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(
        max(dot(viewDir, reflectDir), 0.0),
        material.shininess
    );

    float distance = length(light.position - fragPos);

    float attenuation = 1.0 /
        (light.constant +
         light.linear * distance +
         light.quadratic * distance * distance);

    float theta = dot(
        lightDir,
        normalize(-light.direction)
    );

    float epsilon = light.cutOff - light.outerCutOff;

    float intensity = clamp(
        (theta - light.outerCutOff) / epsilon,
        0.0,
        1.0
    );

    vec3 ambient = light.ambient *
        texture(material.diffuse, TexCoords).rgb;

    vec3 diffuse = light.diffuse * diff *
        texture(material.diffuse, TexCoords).rgb;

    vec3 specular = light.specular * spec *
        texture(material.specular, TexCoords).rgb;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}


// ==========================================
// FUNCION PRINCIPAL
// ==========================================
void main()
{
    vec3 norm = normalize(Normal);

    vec3 viewDir = normalize(viewPos - FragPos);

    // Luz direccional
    vec3 result = CalcDirLight(
        dirLight, norm, viewDir
    );

    // Luces puntuales
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(
            pointLights[i],
            norm,
            FragPos,
            viewDir
        );
    }

    // Luz del reflector
    result += CalcSpotLight(
        spotLight,
        norm,
        FragPos,
        viewDir
    );

    // Obtener el canal alfa de la textura
    vec4 texColor = texture(
        material.diffuse,
        TexCoords
    );

    // Descartar fragmentos completamente transparentes
    if (transparency == 1 && texColor.a < 0.1)
    {
        discard;
    }

    // Transparencia del modelo
    float alpha = clamp(objectAlpha, 0.0, 1.0);

    if (transparency == 1)
    {
        alpha *= texColor.a;
    }

    // Color final
    color = vec4(result, alpha);
}
