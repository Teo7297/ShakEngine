#version 130

#define WHITE vec4(1.0, 1.0, 1.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float u_time;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 texel = texture2D(u_texture, uv);
    vec4 vertexColor = gl_Color;
    vertexColor.a = texel.a;

    vec4 halo = vec4(0.0, 0.0, 0.0, 1.0);
    halo.r = max(sin(u_time * 70), cos(u_time * 70)) * 5;
    vertexColor.r = mix(vertexColor.r, halo.r, 0.8);

    gl_FragColor = vertexColor * texel;
}