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

uniform mat4 WorldMatrix;		// object_to_world
uniform mat4 ViewMatrix;		// world_to_view
uniform mat4 ProjectionMatrix;	// view_to_clip
uniform mat3 NormalMatrix;		// normal_to_view

out vec3 vPosition;

void main() {
    //Vertex.normal_vs = NormalMatrix * normal;
    //Vertex.diffuse = diffuse;
    //Vertex.position = ProjectionMatrix * ViewMatrix * WorldMatrix * vec4(position, 1.0);

    vPosition = position.xyz;
}
