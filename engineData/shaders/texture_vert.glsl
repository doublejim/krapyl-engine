#version 120

attribute vec3 a_position;
attribute vec2 a_texCoord0;
varying vec2 v_texCoord0;
uniform mat4 u_view;

void main()
{
    v_texCoord0 = a_texCoord0;
    vec4 projectedPosition = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(a_position.xyz, 1.0);
    gl_Position = projectedPosition;
}
