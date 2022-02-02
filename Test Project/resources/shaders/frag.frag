#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;
#ifdef VERTEX_COLOR
in vec3 Color;
#endif

// #define NUM_TEXTURE_TYPES 4
// #define DIFFUSE 0
// #define SPECULAR 1
// #define NORMAL 2
// #define HEIGHT 3

struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	sampler2D diffuseTexture;
	sampler2D specularTexture;
	sampler2D normalTexture;
	sampler2D heightTexture;
};


struct DirLight {
    vec3 direction;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
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



//Uniforms
uniform vec3 camPos;
uniform DirLight dirLight;
uniform Material material;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    

	// properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camPos - FragPos);
    vec3 diffResult = CalcDirLight(dirLight, norm, viewDir);  

	// vec3 texRes = texture(material.diffuseTexture, TexCoords).xyz;
	// texRes += texture(material.specularTexture, TexCoords).xyz;
	// texRes += texture(material.normalTexture, TexCoords).xyz;
	// texRes += texture(material.heightTexture, TexCoords).xyz;
	
	#ifdef VERTEX_COLOR
	
	#else
	FragColor = vec4(diffResult, 1.0);
	// FragColor = vec4(texRes + diffResult, 1.0);
	#endif
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
	// diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	// specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);


	// For models without textures
	// vec3 ambient = material.ambientColor;
    // vec3 diffuse = material.diffuseColor * diff;
    // vec3 specular = material.specularColor * spec;

	// For models with textures
	vec3 ambient = vec3(texture(material.diffuseTexture, TexCoords)) * material.ambientColor;
    vec3 diffuse = material.diffuseColor * diff * vec3(texture(material.diffuseTexture, TexCoords));
    vec3 specular = material.specularColor * spec * vec3(texture(material.specularTexture, TexCoords));


	vec3 intensity = vec3(2.0f);
	vec3 res = (ambient + diffuse + specular) * intensity;

    return res;
}