#version 120
uniform sampler2D Texture;
varying vec2 Frag_UV;

void main()
{
    vec4 col = texture2D(Texture, Frag_UV.st);
    
    gl_FragColor = col;
};
