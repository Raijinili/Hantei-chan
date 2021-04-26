#version 120
//uniform sampler2D Texture;
varying vec4 Frag_Color;

void main()
{
    gl_FragColor = Frag_Color; //* texture2D(Texture, Frag_UV.st);
};
