#version 130

#define WHITE vec4(1.0, 1.0, 1.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float u_time;
uniform float u_effectStrenght = 0.2;
uniform float u_effectSpeed = 4.0;

#define S(r,v,a) smoothstep(a/u_resolution.y,0.,abs(v-(r)))

const vec2 s = vec2(1, 1.7320508); // 1.7320508 = sqrt(3)

float calcHexDistance(vec2 p)
{
    p = abs(p);
    return max(dot(p, s * .5), p.x);
}

vec2 calcHexOffset(vec2 uv)
{
    vec4 hexCenter = round(vec4(uv, uv - vec2(.5, 1.)) / s.xyxy);
    vec4 offset = vec4(uv - hexCenter.xy * s, uv - (hexCenter.zw + .5) * s);
    return dot(offset.xy, offset.xy) < dot(offset.zw, offset.zw) ? offset.xy : offset.zw;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = (2. * fragCoord - u_resolution.xy) / u_resolution.y;
    vec2 hexInfo = calcHexOffset(uv * 3.);

    float a = cos(2. * (2. * length(uv) - u_time * u_effectSpeed));
    float h = calcHexDistance(hexInfo);

    fragColor = vec4(0.0);
    fragColor.g = S(abs(sin(h * a * 10.)), 1., 12.) + .3 * S(h, .45, 20.) + .15 + .3 * smoothstep(.25 + 12. / u_resolution.y, .25, h);
    fragColor.a = 1.;
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 texel = texture2D(u_texture, uv);

    vec4 vertexColor = gl_Color;
    mainImage(vertexColor, gl_FragCoord.xy);
    
    vertexColor.a = texel.a;
    gl_FragColor = vertexColor * u_effectStrenght + texel;
}