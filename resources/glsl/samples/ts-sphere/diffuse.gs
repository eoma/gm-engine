#version 410 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in TES_OUT
{
    vec3 diffuse;
    vec3 normal;
    vec3 patchDistance;
} gs_in[3];

out GS_OUT
{
    vec3 diffuse;
    vec3 normal;
    vec3 patchDistance;
    vec3 triDistance;
} gs_out;

void main()
{
    gs_out.diffuse = gs_in[0].diffuse;
    gs_out.normal = gs_in[0].normal;
    gs_out.patchDistance = gs_in[0].patchDistance;
    gs_out.triDistance = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();
    
    gs_out.diffuse = gs_in[1].diffuse;
    gs_out.normal = gs_in[1].normal;
    gs_out.patchDistance = gs_in[1].patchDistance;
    gs_out.triDistance = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();
    
    gs_out.diffuse = gs_in[2].diffuse;
    gs_out.normal = gs_in[2].normal;
    gs_out.patchDistance = gs_in[2].patchDistance;
    gs_out.triDistance = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();
    
    EndPrimitive();
}

