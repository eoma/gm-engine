#version 330 

in vec3 position_ws;

out vec4 color;
uniform vec3 diffuse = vec3(0,0,0);

uniform mat4 ShadowVP = mat4(0);
uniform sampler2DShadow ShadowTexture;

void main() {
	float visibility = 1.0f;
	float bias = 0.005f;

	vec4 shadow_coord = ShadowVP * vec4(position_ws,1);
	float depthTest = textureProj(ShadowTexture, shadow_coord, bias);

	if (depthTest < 1.0)
	{
		visibility = 0.5;
	}

	color = visibility * vec4(diffuse,1);
}
