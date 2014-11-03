#version 410 core

in vec3 tePosition;
in vec3 tePatchDistance;

out vec3 color;

void main() {
    vec3 test1 = tePosition;
    vec3 test2 = tePatchDistance;
    color = vec3(1, 0, 0);
}
