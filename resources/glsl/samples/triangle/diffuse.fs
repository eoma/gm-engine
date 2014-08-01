#version 330 
// TODO: Rewrite to accept a uniform color input here instead of setting to red.
out vec3 color;
uniform vec3 diffuse = vec3(0,0,0);
void main() {
	color = diffuse;
}
