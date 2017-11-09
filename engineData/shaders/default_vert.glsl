#version 120

attribute vec3 a_position;
uniform float u_time;
uniform float u_startTime;

void main()
{
        gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(a_position, 1.0);
}
