//# this is just an example of a geometry shader that creates a quad from a single point. hw is efficient with 1:4 expansion but pre made quads are still better for performance

#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 vertexColor[]; // Color from the vertex shader
in vec2 vertexTexCoord[]; // Texture coordinates from the vertex shader

out vec4 fragColor;
out vec2 texCoord;

uniform float size; // Size of the quad

void main()
{
    vec4 center = gl_in[0].gl_Position;
    vec4 color = vertexColor[0];
    vec2 texCoordCenter = vertexTexCoord[0];

    // Define the four corners of the quad
    vec4 offset[4] = vec4[4](
        vec4(-size, -size, 0.0, 0.0),
        vec4(size, -size, 0.0, 0.0),
        vec4(-size, size, 0.0, 0.0),
        vec4(size, size, 0.0, 0.0)
    );

    // Define the texture coordinates for the quad
    vec2 texCoords[4] = vec2[4](
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(0.0, 1.0),
        vec2(1.0, 1.0)
    );

    for (int i = 0; i < 4; ++i)
    {
        gl_Position = center + offset[i];
        fragColor = color;
        texCoord = texCoords[i];
        EmitVertex();
    }

    EndPrimitive();
}