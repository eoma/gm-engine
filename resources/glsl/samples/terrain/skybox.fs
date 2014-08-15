#version 330 

in vec3 cube_texcoord;
out vec3 color;

uniform samplerCube cube_map;

void main() {
    color = texture(cube_map, cube_texcoord).rgb;
}
