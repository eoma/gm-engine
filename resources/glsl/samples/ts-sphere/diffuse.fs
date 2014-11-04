#version 410 core

in GS_OUT
{
    vec3 diffuse;
    vec3 normal;
    vec3 patchDistance;
    vec3 triDistance;
} fs_in;

out vec3 color;

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1 - exp2(-2*d*d);
    return d;
}

void main()
{
    float d1 = min(min(fs_in.triDistance.x, fs_in.triDistance.y), fs_in.triDistance.z);
    float d2 = min(min(fs_in.patchDistance.x, fs_in.patchDistance.y), fs_in.patchDistance.z);
    
    color = amplify(d1, 60, -0.5) * amplify(d2, 80, -0.5) * fs_in.diffuse;
}
