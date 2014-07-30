#version 330
#define POSITION 0
layout(location = POSITION) in vec3 position;
void main() {
	gl_Position = vec4(position, 1.0);
}