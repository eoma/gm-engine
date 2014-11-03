#version 410 core 

in block
{
    vec3 normal_vs;
    vec3 color;
} Fragment;

out vec3 color;
uniform vec3 diffuse = vec3(1,0,0);

void main() {
    if (Fragment.color == vec3(0, 0, 0))
    {
        color = diffuse;
    }
    else
    {
        color = Fragment.color;
    }
}
