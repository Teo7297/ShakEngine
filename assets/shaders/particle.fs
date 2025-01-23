#version 120

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float u_time;

uniform float u_noiseFrequency = 50.f; // New uniform for noise frequency
uniform float u_noiseAmplitude = 20.f; // New uniform for noise amplitude

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
           (c - a) * u.y * (1.0 - u.x) +
           (d - b) * u.x * u.y;
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 vertexColor = gl_Color;
    
    float n = noise((1 - uv) * u_noiseFrequency + u_time * 25.5);
    n *= u_noiseAmplitude;

    vec4 texel = texture2D(u_texture, uv) * n * 5;

    gl_FragColor = vertexColor * texel;
}