#version 330
#define POSITION 0
#define NORMAL 1
#define TEXCOORD 2
layout(location = POSITION) in vec3 position;
layout(location = NORMAL) in vec3 normal;
layout(location = TEXCOORD) in vec3 texcoord;
void main() {
	gl_Position = vec4(position * 0.001f, 1.0);
}