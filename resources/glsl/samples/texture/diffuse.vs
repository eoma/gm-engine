#version 330
#define POSITION 0
#define TEXCOORD 1
layout(location = POSITION) in vec3 position;
layout(location = TEXCOORD) in vec2 texcoord;
out vec2 tex_coord;
void main() {
	gl_Position = vec4(position, 1.0);
	tex_coord = texcoord;
}