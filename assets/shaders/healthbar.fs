#version 130

#define WHITE vec4(1.0, 1.0, 1.0, 1.0)
#define BLACK vec4(0.0, 0.0, 0.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)

uniform vec2 u_quadOrigin = vec2(0, 0);
uniform vec2 u_size = vec2(300, 10);
uniform float u_hp = 0.50;

vec4 barColorFromHP(float hp, vec2 uv, vec4 background)
{
    vec4 c1 = RED;
    vec4 c2 = GREEN;

    vec4 healthColor = mix(c1, c2, hp);
    if(uv.x > 1)
        return WHITE;
    if(uv.x < hp)
        return healthColor;

    return background;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = (gl_FragCoord.xy - u_quadOrigin) / u_size;
    fragColor = barColorFromHP(u_hp, uv, BLACK);
}

void main()
{
    vec4 vertexColor = gl_Color;
    mainImage(vertexColor, gl_FragCoord.xy);

    gl_FragColor = vertexColor;
}