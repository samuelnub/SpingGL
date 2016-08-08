#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * (2.0 / log2(10000000000.0 + 1.0)) - 1.0;
	gl_Position = projection * view * model * vec4(position, 1.0f); //its backwards, remember y'dumbo!
	//glEnable(GL_PROGRAM_POINT_SIZE); needs to be there!
	TexCoords = vec2(texCoords.x, 1.0f - texCoords.y);
}