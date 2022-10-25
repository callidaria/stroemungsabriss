#version 330 core

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;
uniform sampler2D overlay;

void main()
{
    // textures
    vec4 splash_tex = texture(tex,TexCoords);
    vec4 overlay_tex = texture(overlay,TexCoords);

    // output pixel colour
    outColour = splash_tex;
    outColour *= 1-length(overlay_tex.rgb);
    outColour += overlay_tex*length(splash_tex.rgb);
}