#version 330 

in vec3 normal_in_viewspace;

out vec3 color;

uniform vec3 diffuse = vec3(0,0,0);

void main() {
    float diffuse_intensity = max(0.0, dot(normalize(normal_in_viewspace), -vec3(0,-1,0))); 
	color = diffuse * diffuse_intensity;
}
