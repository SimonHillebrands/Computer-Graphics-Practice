#version 330 core
layout (location = 0) in vec3 aPos; // position has attribute position 0
layout (location = 1) in vec3 aColor;
//out vec4 vertexColor; // specify a color output to the fragment shader
out vec3 ourColor;
void main()
{
gl_Position = vec4(aPos.xyz, 1.0); // we give a vec3 to vec4’s constructor
ourColor = aColor;
//vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // output variable to dark-red
}
