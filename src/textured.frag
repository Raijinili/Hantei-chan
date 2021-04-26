#version 120
uniform sampler2D Texture;
varying vec2 Frag_UV;

void main()
{
    gl_FragColor = texture2D(Texture, Frag_UV.st);
};
