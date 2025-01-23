#version 120

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float u_time;

uniform float u_noiseFrequency = 50.; // New uniform for noise frequency
uniform float u_noiseAmplitude = 20.; // New uniform for noise amplitude

#define WHITE vec4(1.0, 1.0, 1.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)

#define res_          u_resolution
#define time_         u_time
#define detail_steps_ 13
#define mod3_      vec3(.1031, .11369, .13787)

vec3 hash3_3(vec3 p3)
{
    p3 = fract(p3 * mod3_);
    p3 += dot(p3, p3.yxz + 19.19);
    return -1. + 2. * fract(vec3((p3.x + p3.y) * p3.z, (p3.x + p3.z) * p3.y, (p3.y + p3.z) * p3.x));
}

float perlin_noise3(vec3 p)
{
    vec3 pi = floor(p);
    vec3 pf = p - pi;

    vec3 w = pf * pf * (3. - 2. * pf);

    return mix(mix(mix(dot(pf - vec3(0, 0, 0), hash3_3(pi + vec3(0, 0, 0))), dot(pf - vec3(1, 0, 0), hash3_3(pi + vec3(1, 0, 0))), w.x), mix(dot(pf - vec3(0, 0, 1), hash3_3(pi + vec3(0, 0, 1))), dot(pf - vec3(1, 0, 1), hash3_3(pi + vec3(1, 0, 1))), w.x), w.z), mix(mix(dot(pf - vec3(0, 1, 0), hash3_3(pi + vec3(0, 1, 0))), dot(pf - vec3(1, 1, 0), hash3_3(pi + vec3(1, 1, 0))), w.x), mix(dot(pf - vec3(0, 1, 1), hash3_3(pi + vec3(0, 1, 1))), dot(pf - vec3(1, 1, 1), hash3_3(pi + vec3(1, 1, 1))), w.x), w.z), w.y);
}

float noise_sum_abs3(vec3 p)
{
    float f = 0.;
    p = p * 3.;
    f += 1.0000 * abs(perlin_noise3(p));
    p = 2. * p;
    f += 0.5000 * abs(perlin_noise3(p));
    p = 3. * p;
    f += 0.2500 * abs(perlin_noise3(p));
    p = 4. * p;
    f += 0.1250 * abs(perlin_noise3(p));
    p = 5. * p;
    f += 0.0625 * abs(perlin_noise3(p));
    p = 6. * p;

    return f;
}

vec2 domain(vec2 uv, float s)
{
    return (2. * uv.xy - res_.xy) / res_.y * s;
}

float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise(vec2 st)
{
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

    // float n = noise((1 - uv) * u_noiseFrequency + u_time * 25.5);
    // n *= u_noiseAmplitude;

    vec4 texel = texture2D(u_texture, uv);
    if(texel.a > 0.)
        texel = WHITE;

    // gl_FragColor = vertexColor * texel * BLUE;
    vec2 p = domain(gl_FragCoord.xy, 2.5);

    float electric_density = .9;
    float electric_radius = length(p) - 2;
    float velocity = .1;

    float moving_coord = sin(velocity * time_) / .2 * cos(velocity * time_);
    vec3 electric_local_domain = vec3(p.xy, moving_coord);
    float electric_field = electric_density * noise_sum_abs3(electric_local_domain);

    vec3 col = vec3(107, 148, 196) / 255.;
    col += vec3(1 - (electric_field + electric_radius));

    for(int i = 0; i < detail_steps_; i++)
    {
        if(length(col) >= 2.1 + float(i) / 2.)
            col -= .3;
    }
    col += vec3(1) - vec3(4.2 * electric_field);
    float alpha = 1.;

    gl_FragColor = vec4(col, alpha);

    /////////////////////////////////////

    // Zooms out by a factor of 2.0
    uv *= 2.0;
    // Shifts every axis by -1.0
    uv -= 1.0;

    // Base color for the effect
    vec3 finalColor = vec3(.2, 1., 0.);

    // finalColor *= abs(0.05 / (sin( uv.x + sin(uv.y+u_time)* 0.3 ) * 20.0) );
    gl_FragColor *= vec4(finalColor, 1) * texel;
}