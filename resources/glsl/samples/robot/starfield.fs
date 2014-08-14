// Based on Morgan McGuire's Starfield shader that's based on Star Nest by Kali 
// http://casual-effects.blogspot.no/2013/08/starfield-shader.html
// https://www.shadertoy.com/view/4dfGDM
// That shader and this one are open source under the MIT license
//
// Assumes an sRGB target (i.e., the output is already encoded to gamma 2.1)
#version 330
 
in vec2 texcoord;
out vec4 color;
 
// viewport resolution (in pixels)
uniform vec2 resolution;
uniform vec2 inv_resolution;
 
// In the noise-function space. xy corresponds to screen-space XY
uniform vec3 origin = vec3(0,0,0);
uniform mat2 rotate = mat2(1);

uniform float zoom = 1.0;

// FIXME: Requires FBO/RTT
//uniform sampler2D oldImage;
 
#define iterations 17
#define volsteps 8
#define sparsity 0.425  // .4 to .5 (sparse)
#define stepsize 0.205
#define frequency_variation   1.9 // 0.5 to 2.0
#define brightness 0.0021
#define distfading 0.6800
 
void main() {    
    vec2 uv = gl_FragCoord.xy * inv_resolution - 0.5;
    uv.y *= resolution.y * inv_resolution.x;
 
    vec3 dir = vec3(uv * zoom, 1.0);
    dir.xz *= rotate;
 
    float s = 0.1, fade = 0.01;
    color.rgb = vec3(0.0);
     
    for (int r = 0; r < volsteps; ++r) {
        vec3 p = origin + dir * (s * 0.5);
        p = abs(vec3(frequency_variation) - mod(p, vec3(frequency_variation * 2.0)));
 
        float prevlen = 0.0, a = 0.0;
        for (int i = 0; i < iterations; ++i) {
            p = abs(p);
            p = p * (1.0 / dot(p, p)) + (-sparsity); // the magic formula            
            float len = length(p);
            a += abs(len - prevlen); // absolute sum of average change
            prevlen = len;
        }
         
        a *= a * a; // add contrast
         
        // coloring based on distance        
        color.rgb += (vec3(s, s*s, s*s*s) * a * brightness + 1.0) * fade;
        fade *= distfading; // distance fading
        s += stepsize;
    }
     
    color.rgb = min(color.rgb, vec3(1.2));
 
    // Detect and suppress flickering single pixels (ignoring the huge gradients that we encounter inside bright areas)
    float intensity = min(color.r + color.g + color.b, 0.7);
 
    ivec2 sgn = (ivec2(gl_FragCoord.xy) & 1) * 2 - 1;
    vec2 gradient = vec2(dFdx(intensity) * sgn.x, dFdy(intensity) * sgn.y);
    float cutoff = max(max(gradient.x, gradient.y) - 0.1, 0.0);
    color.rgb *= max(1.0 - cutoff * 6.0, 0.3);
 
    // Motion blur; increases temporal coherence of undersampled flickering stars
    // and provides temporal filtering under true motion.  
    //vec3 oldValue = texelFetch(oldImage, gl_FragCoord.xy, 0).rgb;
    //color.rgb = lerp(oldValue - vec3(0.004), color.rgb, 0.5);
    color.a = 1.0;
}