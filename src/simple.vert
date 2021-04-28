#version 120
attribute vec2 Position;
attribute vec3 Color;

varying vec4 Frag_Color;

uniform mat4 ProjMtx;

void main()
{
    
    Frag_Color = vec4(Color, 0.8);
    gl_Position = ProjMtx * vec4(Position, -1, 1);
};
