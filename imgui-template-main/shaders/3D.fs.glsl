#version 330 core

in vec3 vPosition_vs;
in vec3 vNormal_vs;

// Nouvelle variable qu'on ajoute pour dire si la case est noire ou blanche
uniform vec3 uColor; 

out vec4 fFragColor;

void main() {
    // On colorie simplement la case pour le moment
    fFragColor = vec4(uColor, 1.0); 
}