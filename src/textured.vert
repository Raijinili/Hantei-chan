#version 120
attribute vec2 Position;
attribute vec2 UV;

varying vec2 Frag_UV;

uniform mat4 ProjMtx;

void main()
{
    Frag_UV = UV;
    gl_Position = ProjMtx * vec4(Position.xy, 0, 1);
};
