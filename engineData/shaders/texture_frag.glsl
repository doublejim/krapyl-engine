#version 120

uniform vec4 u_color;
uniform sampler2D u_sampler;
varying vec2 v_texCoord0;

void main()
{
    gl_FragColor = texture2D(u_sampler, vec2(v_texCoord0.s, 1.0 - v_texCoord0.t)) + u_color;
}
