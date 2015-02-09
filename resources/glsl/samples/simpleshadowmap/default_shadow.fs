#version 330 

layout(location = 0) out vec3 fragmentDepth;

void main() {
	fragmentDepth = gl_FragCoord.zzz;
}
