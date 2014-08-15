#version 410 core
#define DIFFUSE		0
#define POSITION	1
#define NORMAL		2
#define TEXCOORD	3
#define TANGENT		4
#define BITANGENT	5

layout(location = POSITION) in vec3 position;
layout(location = NORMAL) in vec3 normal;

out vec3 control_point;
out vec3 control_normal;
out vec2 control_texcoord;

uniform mat4 WorldMatrix;		// object_to_world
uniform sampler2D heightmap;
    
void main(void){
	control_texcoord = position.xz;
	control_normal = normal;

    float height = texture(heightmap, control_texcoord.yx).x;
    control_point = (WorldMatrix * vec4(position.x, height, position.z, 1)).xyz;
}