#version 330 

in vec2 tex_coord;
out vec3 color;
uniform vec3 diffuse = vec3(0,0,0);
uniform sampler2D diffuse_map;
void main() {
	color = diffuse * texture(diffuse_map, tex_coord).rgb;
    //color = diffuse;
}
