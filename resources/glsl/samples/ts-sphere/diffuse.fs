#version 410 core

in vec3 tePosition;
in vec3 tePatchDistance;
in vec3 diffuse;

out vec3 color;

void main() {
    vec3 test1 = tePosition;
    vec3 test2 = tePatchDistance;
    color = diffuse;
}
