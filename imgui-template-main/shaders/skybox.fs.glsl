#version 330 core
in vec2 vTexCoords;
out vec4 fFragColor;

uniform sampler2D uSkyboxTexture; // Retour au sampler2D classique
uniform float uDayNightFactor;

void main() {
    vec4 texColor = texture(uSkyboxTexture, vTexCoords);
    vec3 nightTint = vec3(0.05, 0.05, 0.15);
    fFragColor = vec4(mix(texColor.rgb, texColor.rgb * nightTint, uDayNightFactor), 1.0);
}