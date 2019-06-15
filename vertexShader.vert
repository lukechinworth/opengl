#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform vec2 offset;
uniform mat4 perspectiveMatrix;

smooth out vec4 vertexColor;

void main()
{
    vec4 cameraPosition = position + vec4(offset.x, offset.y, 0.0f, 0.0f);

    gl_Position = perspectiveMatrix * cameraPosition;
    vertexColor = color;
}