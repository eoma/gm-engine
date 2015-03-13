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

layout(std140) uniform CameraMatrices {
	mat4 ViewMatrix;		// world_to_view
	mat4 ProjectionMatrix;	// view_to_clip
};

uniform mat4 WorldMatrix;		// object_to_world
uniform mat3 NormalMatrix;		// normal_to_view

out gl_PerVertex
{
    vec4 gl_Position;
};

out block
{
    vec3 normal_vs;
    vec3 color;
} Vertex;

void main() {
    Vertex.normal_vs = NormalMatrix * normal;
    Vertex.color = vec3(0, 0, 0);
    
    gl_Position = ProjectionMatrix * ViewMatrix * WorldMatrix * vec4(position, 1.0);
}
