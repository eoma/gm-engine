#version 410 core
#define NORMAL_LENGTH 10

layout(triangles, invocations = 2) in;
layout(line_strip, max_vertices = 12) out;

in block
{
    vec3 normal_vs;
    vec3 color;
} Vertex[];

out block
{
    vec3 normal_vs;
    vec3 color;
} Fragment;

void main()
{
    // Original
    if (gl_InvocationID == 0)
    {
        Fragment.normal_vs = Vertex[0].normal_vs;
        Fragment.color = Vertex[0].color;
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        Fragment.normal_vs = Vertex[1].normal_vs;
        Fragment.color = Vertex[1].color;
        gl_Position = gl_in[1].gl_Position;
        EmitVertex();
        EndPrimitive();
        
        Fragment.normal_vs = Vertex[1].normal_vs;
        Fragment.color = Vertex[1].color;
        gl_Position = gl_in[1].gl_Position;
        EmitVertex();
        Fragment.normal_vs = Vertex[2].normal_vs;
        Fragment.color = Vertex[2].color;
        gl_Position = gl_in[2].gl_Position;
        EmitVertex();
        EndPrimitive();
        
        Fragment.normal_vs = Vertex[2].normal_vs;
        Fragment.color = Vertex[2].color;
        gl_Position = gl_in[2].gl_Position;
        EmitVertex();
        Fragment.normal_vs = Vertex[0].normal_vs;
        Fragment.color = Vertex[0].color;
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
    
    // Normals
    if (gl_InvocationID == 1)
    {
        for (int i = 0; i < 3; ++i)
        {
            Fragment.normal_vs = Vertex[i].normal_vs;
            Fragment.color = vec3(1, 0, 0);
            gl_Position = gl_in[i].gl_Position;
            EmitVertex();
            
            Fragment.normal_vs = Vertex[i].normal_vs;
            Fragment.color = vec3(1, 0, 0);
            gl_Position = gl_in[i].gl_Position + vec4(normalize(Vertex[i].normal_vs) * NORMAL_LENGTH, 1.0);
            EmitVertex();
            
            EndPrimitive();
        }
    }
}