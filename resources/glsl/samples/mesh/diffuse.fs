#version 330 

out vec3 color;
uniform vec3 diffuse = vec3(0,0,0);
void main() {
	color = diffuse;
}
