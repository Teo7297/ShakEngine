#version 130

#define WHITE vec4(1.0, 1.0, 1.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float u_time;
uniform float u_hp = 0.75;

vec3 barColorFromHP(float hp, vec2 uv, vec3 background)
{

    //some variables or readibility
    vec3 c1 = vec3(1, 0, 0); //RED
    vec3 c2 = vec3(0, 1, 0); //GREEN

    //the entire bar color is determined by the current hp percentage
    vec3 healthColor = mix(c1, c2, hp);

    //visualize the hp with the x parameter
    if(uv.x < hp)
        return healthColor;

    //otherwise it is the background
    return background;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec3 black = vec3(0, 0, 0);

    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord / u_resolution.xy;

    // Derive the bar color from the health and use background=black
    vec3 color3 = barColorFromHP(u_hp, uv, black);

    // Output to screen
    fragColor = vec4(color3, 1.0);
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec4 texel = texture2D(u_texture, uv);

    vec4 vertexColor = gl_Color;
    mainImage(vertexColor, gl_FragCoord.xy);

    gl_FragColor = vertexColor;
}