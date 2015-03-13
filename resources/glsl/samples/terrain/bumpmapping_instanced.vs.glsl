#version 410 core

#define DIFFUSE		0
#define POSITION	1
#define NORMAL		2
#define TEXCOORD	3
#define TANGENT		4
#define BITANGENT	5

layout (location = POSITION) in vec4 position;
layout (location = NORMAL) in vec3 normal;
layout (location = TANGENT) in vec3 tangent;
// Although the model file used in this example includes
// a bitangent, we're not using it and will calculate our
// own from the normal and tangent.
// layout (location = 3) in vec3 bitangent;
layout (location = TEXCOORD) in vec2 texcoord;

layout(location = 6) in vec3 instance_position;

out VS_OUT
{
    vec2 texcoord;
    vec3 eyeDir;
    vec3 lightDir;
    vec3 normal;
} vs_out;

layout(std140) uniform CameraMatrices {
	mat4 ViewMatrix;		// world_to_view
	mat4 ProjectionMatrix;	// view_to_clip
};

uniform mat4 ObjectMatrix;
//uniform mat4 WorldMatrix;		// object_to_world, WorldMatrix = ParentWorldMatrix * ObjectMatrix
uniform mat4 ParentWorldMatrixNoScale;		// 

//uniform mat4 mv_matrix;
//uniform mat4 proj_matrix;
uniform vec3 light_pos = vec3(0.0, 0.0, 100.0);

void main(void)
{
    mat4 translation = mat4(1);
    translation[3].xyz = instance_position;

    mat4 mv_matrix = ViewMatrix * ParentWorldMatrixNoScale * translation * ObjectMatrix;
    mat4 proj_matrix = ProjectionMatrix;

    // Calculate vertex position in view space.
    vec4 P = mv_matrix * position;

    // Calculate normal (N) and tangent (T) vectors in view space from
    // incoming object space vectors.
    vec3 V = P.xyz;
    vec3 N = normalize(mat3(mv_matrix) * normal);
    vec3 T = normalize(mat3(mv_matrix) * tangent);
    // Calculate the bitangent vector (B) from the normal and tangent
    // vectors.
    vec3 B = cross(N, T);

    // The light vector (L) is the vector from the point of interest to
    // the light. Calculate that and multiply it by the TBN matrix.
    vec3 L = light_pos - P.xyz;
    vs_out.lightDir = normalize(vec3(dot(L, T), dot(L, B), dot(L, N)));

    // The view vector is the vector from the point of interest to the
    // viewer, which in view space is simply the negative of the position.
    // Calculate that and multiply it by the TBN matrix.
    V = -P.xyz;
    vs_out.eyeDir = normalize(vec3(dot(V, T), dot(V, B), dot(V, N)));

    // Pass the texture coordinate through unmodified so that the fragment
    // shader can fetch from the normal and color maps.
    vs_out.texcoord = texcoord;

    // Pass the per-vertex normal so that the fragment shader can
    // turn the normal map on and off.
    vs_out.normal = N;

    // Calculate clip coordinates by multiplying our view position by
    // the projection matrix.
    gl_Position = proj_matrix * P;
}
