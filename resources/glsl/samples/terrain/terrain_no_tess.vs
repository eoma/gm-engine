#version 410 core
#define DIFFUSE		0
#define POSITION	1
#define NORMAL		2
#define TEXCOORD	3
#define TANGENT		4
#define BITANGENT	5

layout(location = POSITION) in vec3 position;
layout(location = NORMAL) in vec3 normal;

out vec4 position_in_viewspace;
out vec3 normal_in_viewspace;
out vec2 texcoord;

layout(std140) uniform CameraMatrices {
	mat4 ViewMatrix;		// world_to_view
	mat4 ProjectionMatrix;	// view_to_clip
};

uniform mat4 WorldMatrix;		// object_to_world
uniform mat3 NormalMatrix;		// normal_to_view

uniform sampler2D heightmap;

vec3 approximateTangent(in vec3 normal)
{
	vec3 tangent;
	vec3 c1 = cross(normal, vec3(0.0, 0.0, 1.0)); 
	vec3 c2 = cross(normal, vec3(0.0, 1.0, 0.0)); 
	if(length(c1) > length(c2))
	{
		tangent = c1;	
	}
	else
	{
		tangent = c2;	
	}
	return tangent;
}
    
void main(void){
	texcoord = position.xz;

	// Example on use of approximating tangent for terrain/grids. Merge with bumpmap shader for adding support for bump maps on terrain textures...
	//vec3 tangent = approximateTangent(normal);
	//vec3 bitangent = cross(normal, tangent);
	//vec3 tangent_in_viewspace = NormalMatrix * tangent;
	//vec3 bitangent_in_viewspace = NormalMatrix * bitangent;

	normal_in_viewspace = NormalMatrix * normal;


    float height = texture(heightmap, texcoord.yx).y;
    position_in_viewspace = ViewMatrix * WorldMatrix * vec4(position.x, height, position.z, 1);
	gl_Position = ProjectionMatrix * position_in_viewspace;
}
