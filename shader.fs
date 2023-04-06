#version 330 core
out vec4 FragColor;

float animationSeconds = 2.0; // how long do we want the animation to last before looping
float piTimes2 = 3.1415926536*2.0;

uniform vec2 resolution = vec2(800,600);
uniform float time;

void main()
{

    vec2 uv = (2.0*gl_FragCoord.xy-resolution.xy)/resolution.y;

    float sineVal = sin(piTimes2*(time-0.75)/animationSeconds)/2.0+0.5; 

    FragColor = vec4(gl_FragCoord.x/resolution.x,sineVal,1.0-gl_FragCoord.y/resolution.y,1.0);
}
