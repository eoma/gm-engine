#version 330
#define DIFFUSE		0
#define POSITION	1
#define NORMAL		2
#define TEXCOORD	3
#define TANGENT		4
#define BITANGENT	5

layout(location = POSITION) in vec3 position;
layout(location = NORMAL) in vec3 normal;
layout(location = TEXCOORD) in vec2 texcoord;

layout(std140) uniform CameraMatrices {
	mat4 ViewMatrix;		// world_to_view
	mat4 ProjectionMatrix;	// view_to_clip
};

uniform mat4 WorldMatrix;		// object_to_world
void main() {
	gl_Position = ProjectionMatrix * ViewMatrix * WorldMatrix * vec4(position, 1.0);
}
