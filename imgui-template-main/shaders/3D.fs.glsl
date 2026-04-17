#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensityDir;
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensityPoint;

uniform sampler2D uTexture;
uniform int uUseTexture;
//  VARIABLE POUR RÉPÉTER L'HERBE :
uniform float uUVScale; 

vec3 calculateLighting(vec3 normal, vec3 diffuseColor) {
    vec3 viewDir = normalize(-vPosition_vs);
    vec3 result = vec3(0.0);

    float diffDir = max(dot(normal, uLightDir_vs), 0.0);
    vec3 diffuseDir = diffDir * uLightIntensityDir * diffuseColor;
    vec3 reflectDir = reflect(-uLightDir_vs, normal);
    float specDir = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);
    vec3 specularDir = specDir * uLightIntensityDir * uKs;
    result += diffuseDir + specularDir;

    vec3 lightPosDir = uLightPos_vs - vPosition_vs;
    float dist = length(lightPosDir);
    vec3 L = normalize(lightPosDir);
    float diffPoint = max(dot(normal, L), 0.0);
    vec3 diffusePoint = (diffPoint * uLightIntensityPoint * diffuseColor) / (dist * dist);
    vec3 reflectPoint = reflect(-L, normal);
    float specPoint = pow(max(dot(viewDir, reflectPoint), 0.0), uShininess);
    vec3 specularPoint = (specPoint * uLightIntensityPoint * uKs) / (dist * dist);
    result += diffusePoint + specularPoint;

    result += diffuseColor * 0.1;
    return result;
}

void main() {
    vec3 normal = normalize(vNormal_vs);
    vec3 baseColor;

    if (uUseTexture == 1) {
        // C'EST ICI QUE L'HERBE SE RÉPÈTE GRÂCE À uUVScale
        baseColor = texture(uTexture, vTexCoords * uUVScale).rgb * uKd;
    } else {
        baseColor = uKd;
    }

    vec3 finalColor = calculateLighting(normal, baseColor);
    fFragColor = vec4(finalColor, 1.0);
}