#version 120
uniform sampler2D Texture;
varying vec2 Frag_UV;

void main()
{
    vec4 col = texture2D(Texture, Frag_UV.st);
    //col.a = 1.f;
    gl_FragColor = col;
};
