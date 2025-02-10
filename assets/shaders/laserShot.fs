#version 120

#define WHITE vec4(1.0, 1.0, 1.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float u_time;
uniform float u_laserWidth = 5.0;
uniform bool u_isRainbow = false;
uniform vec4 u_laserColor = RED;

const float centerIntensity = 100.0;
const float laserStartPercentage = 0.4;
const float speed = 1.8;

void drawLaser(out vec4 fragColor, in vec2 fragCoord)
{
    // Normalize pixel coordinates (from 0 to 1)
    vec2 uv = gl_TexCoord[0].xy;                //! UV texture coords based
    // vec2 uv = fragCoord / u_resolution.xy;   //! UV window coords based

    // this make the laser move horizontally
    // uv.x += (fract(u_time) - 0.5) * 10;

    //Define the laser effect and center color
    vec4 laserColor;
    if(u_isRainbow)
    {
        laserColor.rgb = 0.5 + 0.5 * cos(u_time * 15.0 + uv.xyx + vec3(2, 4, 6));
    }
    else
    {
        laserColor.rgb = vec3(3 + cos(u_time)) * (1 - uv.xyx);
        laserColor *= u_laserColor;
    }
    laserColor.a = 1.0;

    //Calculate how close the current pixel is to the center line of the screen
    float intensity = 1.0 - abs(uv.y - 0.5);

    //Raise it an exponent, resulting in sharply increased intensity at the center that trails off smoothly
    intensity = pow(intensity, 16.0);

    //Make the laser trail off at the start
    if(uv.x < laserStartPercentage)
        intensity = mix(0.0, intensity, pow(uv.x / laserStartPercentage, 0.5));

    //Make the laser trail off at the end
    if(uv.x > 1.0 - laserStartPercentage)
        intensity = mix(0.0, intensity, pow((1.0 - uv.x) / laserStartPercentage, 0.5));

    //Pick where to sample the texture used for the flowing effect
    vec2 samplePoint = uv;

    //Stretch it horizontally and then shift it over time to make it appear to be flowing
    samplePoint.x = samplePoint.x * 0.1 - (u_time * speed);

    //Compress it vertically
    samplePoint.y = samplePoint.y * 4.0;

    //Get the texture at that point
    vec4 sampleIntensity = texture2D(u_texture, samplePoint);
    vec4 sampleColor = laserColor * sampleIntensity;

    //Mix it with 'intensity' to make it more intense near the center
    vec4 effectColor = sampleColor * intensity * u_laserWidth;

    //Mix it with the color white raised to a higher exponent to make the center white beam
    effectColor = effectColor + WHITE * centerIntensity * (pow(intensity, 13.0) * sampleIntensity);

    //Mix the laser color with the center beam
    laserColor = mix(laserColor, effectColor, 0.6);

    //Reduce the brightness of the background to emphacize the laser
    fragColor *= pow(1.0 - intensity, 3.0);

    //Add the laser to the background scene
    fragColor = mix(fragColor, laserColor, intensity * 2.0);
}

void main()
{
    vec4 vertexColor = gl_Color;

    drawLaser(vertexColor, gl_FragCoord.xy);

    gl_FragColor = vertexColor;
}