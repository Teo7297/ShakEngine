#version 130

/*
    "Plasma" by @XorDev
    
    X Post:
    x.com/XorDev/status/1894123951401378051
    
*/

uniform vec2 u_resolution;
uniform float u_time;

void mainImage( out vec4 O, in vec2 I )
{
    //Resolution for scaling
    vec2 r = u_resolution.xy,
    //Centered, ratio corrected, coordinates
    p = (I+I-r) / r.y,
    //Z depth
    z,
    //Iterator (x=0)
    i,
    //Fluid coordinates
    f = p*(z+=4.-4.*abs(.7-dot(p,p)));
    
    //Clear frag color and loop 8 times
    for(O *= 0.; i.y++<8.;
        //Set color waves and line brightness
        O += (sin(f)+1.).xyyx * abs(f.x-f.y))
        //Add fluid waves
        f += cos(f.yx*i.y+i+u_time)/i.y+.7;
    
    //Tonemap, fade edges and color gradient
    O = tanh(7.*exp(z.x-4.-p.y*vec4(-1,1,2,0))/O);
}

void main()
{
    vec2 uv = gl_TexCoord[0].xy;

    vec4 vertexColor = gl_Color;
    float currAlpha = vertexColor.a;

    mainImage(vertexColor, gl_FragCoord.xy);

    vertexColor.a = currAlpha;
    gl_FragColor = vertexColor;
}