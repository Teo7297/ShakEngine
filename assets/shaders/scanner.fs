#version 120

#define WHITE vec4(1.0, 1.0, 1.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float u_time;

#define rand(co) fract(sin(mod(dot(co ,vec2(12.9898,78.233)),3.14))*43758.5453)

float EPSILON = 0.001;

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = fragCoord.xy / u_resolution.xy;
    vec4 pixel = texture2D(u_texture, uv);
    float opaque = smoothstep(.0, 0.7, distance(pixel.rgb, vec3(0.0471, 0.6353, 0.1451)));
    pixel *= opaque;

    float t = u_time * 2;

    int OCTAVES = 4;
    float amplitude = 1.0;
    float value = 0.0;
    vec2 sample_t = vec2(t * 0.00001, 1.0);
    for(int i = 0; i < OCTAVES; i++)
    {
        value += amplitude * (rand(sample_t));
        sample_t *= 2.;
        amplitude *= .5;
    }

    vec3 c = vec3(0.0, 0.0, 0.0);
    for(float i = 0.2; i < 1.0; i += 0.2)
    {

        float f1 = 1.0 / (u_resolution.x * 0.15 * abs(uv.x -
            clamp(fract(t - 0.25) * 4.0, 0.0, 1.1)) + EPSILON);
        // inverse x direction
        float f11 = 1.0 / (u_resolution.x * 0.15 * abs(uv.x -
            clamp(fract(-t) * 4.0, 0.0, 1.1)) + EPSILON);

        float f2 = 1.0 / (u_resolution.y * 0.15 * abs(uv.y -
            clamp(fract(t - 0.5) * 4.0, 0.0, 1.1)) + EPSILON);

        // inverse y direction
        float f22 = 1.0 / (u_resolution.y * 0.15 * abs(uv.y -
            clamp(fract(-t + 0.25) * 4.0, 0.0, 1.1)) + EPSILON);
        c += (f1 + f11) * vec3(0.3, 0.2, i * 0.3) +
            (f2 + f22) * vec3(i * 0.3, 0.2, 0.3);
    }
    fragColor = pixel;
    fragColor += vec4(c, 1.0) * opaque;

    // fragColor = vec4(c, 1.0);

}

void main()
{
    // vec2 uv = gl_TexCoord[0].xy;
    // vec4 texel = texture2D(u_texture, uv);

    vec4 vertexColor = gl_Color;
    mainImage(vertexColor, gl_FragCoord.xy);
    gl_FragColor = vertexColor;
}