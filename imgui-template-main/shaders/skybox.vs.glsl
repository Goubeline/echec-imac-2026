#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aTexCoords; // On remet les UV

out vec2 vTexCoords;
uniform mat4 uMVP;

void main() {
    vTexCoords = aTexCoords;
    gl_Position = uMVP * vec4(aPosition, 1.0);
}