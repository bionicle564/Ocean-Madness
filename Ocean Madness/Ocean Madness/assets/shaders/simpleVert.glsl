#version 430

layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

layout (location = 1) uniform vec4 colour;
uniform mat4 matView;
uniform mat4 matProjection;
  
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
	mat4 MVP = matProjection * matView;
    gl_Position = MVP * vec4(aPos, 1.0); // see how we directly give a vec3 to vec4's constructor
    vertexColor = colour; // set the output variable to a dark-red color
}