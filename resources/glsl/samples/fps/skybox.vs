#version 330
#define DIFFUSE		0
#define POSITION	1
#define NORMAL		2
#define TEXCOORD	3
#define TANGENT		4
#define BITANGENT	5

layout(location = POSITION) in vec3 position;
layout(location = TEXCOORD) in vec3 texcoord;

out vec3 cube_texcoord;

uniform mat4 WorldMatrix;		// object_to_world
uniform mat4 ViewMatrix;		// world_to_view
uniform mat4 ProjectionMatrix;	// view_to_clip

void main() {
	cube_texcoord = texcoord;
	gl_Position = ProjectionMatrix * ViewMatrix * WorldMatrix * vec4(position, 1.0);
}
