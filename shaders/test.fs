uniform sampler2D texture;
uniform vec2 mouse;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // calculate the distance from the current pixel to the mouse position
    vec2 pixelPos = gl_FragCoord.xy;
    float distance = length(pixelPos - mouse);

    // if the pixel is near the mouse, color it blue
    if (distance <= 10.0)
    {
        gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0); // blue color
    }
    else
    {
        // multiply it by the color
        gl_FragColor = gl_Color * pixel;
    }
}