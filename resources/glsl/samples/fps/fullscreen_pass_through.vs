#version 330
#define DIFFUSE		0
#define POSITION	1
#define NORMAL		2
#define TEXCOORD	3
#define TANGENT		4
#define BITANGENT	5
layout(location = POSITION) in vec2 position;
out vec2 texcoord;
void main() {
	texcoord = (position * vec2(0.5,0.5)) + vec2(0.5,0.5);
	gl_Position = vec4( position , 0.0, 1.0);
}
