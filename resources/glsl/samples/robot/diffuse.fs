#version 330 

in vec2 tex_coord;
out vec3 color;

uniform vec3 diffuse = vec3(0,0,0);
uniform sampler2D diffuse_map;
uniform float map_repeat_count = 1.0;

void main() {
	color = diffuse * texture(diffuse_map, tex_coord*map_repeat_count).rgb;
    //color = diffuse;
}
