#version 330 

in vec4 position_in_viewspace;
in vec3 normal_in_viewspace;
out vec3 color;

uniform vec3 diffuse = vec3(0,0,0);

#define MAX_LIGHTS 256

struct LightSource {
	float radius;
	vec3 position_in_viewspace;
	vec3 material_color_diffuse;
	vec3 material_color_specular;
	vec3 material_color_ambient;
};

layout(std140) uniform LightList {
	int light_count;
	LightSource light[MAX_LIGHTS];
};

float compute_blinn_term(in vec3 N, in vec3 L, in vec3 V, in float NdotL, in float shininess)
{
        vec3 H = normalize(L + V);
        float term = dot(N,H);
        term = max(clamp(term, 0.0, 1.0), 0.0);
        term = pow(term, shininess);
        return term;
}

vec3 color_from_light(in int light_index, in vec3 N, in vec3 V, in vec3 diffuse)
{
        vec3 L = (light[light_index].position_in_viewspace - position_in_viewspace.xyz);
        vec3 L_rad = L / light[light_index].radius;
        float light_attenuation = max(0, 1.0 - dot(L_rad,L_rad));
        vec3 light_diffuse = light[light_index].material_color_diffuse;
        vec3 light_specular = light[light_index].material_color_specular;
        vec3 light_ambient = light[light_index].material_color_ambient;
        
        float NdotL = max(dot(N,L), 0.0);
        float shininess = 0.75;
        float term = compute_blinn_term(N, L, V, NdotL, shininess);
        
        //Uncomment to debug light attenuation
        //return vec4(1,1,1,1) * light_attenuation;
        
        return vec3( 
                (diffuse * NdotL * light_diffuse * light_attenuation) + 
                (diffuse * term * light_specular * light_attenuation) + 
                (diffuse * light_ambient * 0.5)
        );
}

void main() {
    vec3 N = normalize(normal_in_viewspace);
    vec3 V = normalize(-position_in_viewspace.xyz);

	color = vec3(0,0,0);
    for(int i = 0; i < light_count; i++)
    {
        color += color_from_light(i, N, V, diffuse);
    }
    if(color == vec3(0,0,0)) {
        color = diffuse;
    }
}
