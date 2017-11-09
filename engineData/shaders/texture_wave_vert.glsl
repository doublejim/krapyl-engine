#version 120

attribute vec3 a_position;
attribute vec2 a_texCoord0;
varying vec2 v_texCoord0;
uniform mat4 u_view;
uniform float u_time;
uniform float u_startTime;

void main()
{
    v_texCoord0 = a_texCoord0;

    float timeDelta = u_time - u_startTime;
    float positionZ = a_position.z + sin(a_position.x + timeDelta) + cos((a_position.y + timeDelta));
    vec4 projectedPosition = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(a_position.xy, positionZ, 1.0);

    gl_Position = projectedPosition;
}
