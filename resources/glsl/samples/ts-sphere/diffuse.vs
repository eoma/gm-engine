#version 410 core
#define DIFFUSE		0
#define POSITION	1
#define NORMAL		2
#define TEXCOORD	3
#define TANGENT		4
#define BITANGENT	5

layout(location = DIFFUSE) in vec3 diffuse;
layout(location = POSITION) in vec3 position;
layout(location = NORMAL) in vec3 normal;

//uniform mat4 WorldMatrix;		// object_to_world
//uniform mat4 ViewMatrix;		// world_to_view
//uniform mat4 ProjectionMatrix;	// view_to_clip
//uniform mat3 NormalMatrix;		// normal_to_view

out vec3 vPosition;
out vec3 vDiffuse;
out vec3 vNormal;

void main() {
    vDiffuse = diffuse;
    vPosition = position;
    vNormal = normal;
}
